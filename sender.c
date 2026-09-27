/*
 * file_sender.c
 *
 * Periodically scans a directory for .txt files, uploads each to an HTTP
 * server via POST (multipart/form-data), and deletes the file on success.
 *
 * Build:
 *   gcc -o file_sender file_sender.c -lcurl
 *
 * Usage:
 *   ./file_sender <watch_directory> <server_url>
 *
 * Example:
 *   ./file_sender /var/log/myapp http://192.168.1.10:8080/upload
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <curl/curl.h>

/* ── Configuration ──────────────────────────────────────────────────── */
#define SCAN_INTERVAL_SEC   60          /* scan every 60 seconds        */
#define FILE_EXTENSION      ".txt"      /* only send .txt files         */
#define MAX_PATH_LEN        4096
#define HTTP_OK_MIN         200         /* inclusive                     */
#define HTTP_OK_MAX         299         /* inclusive                     */

/* ── Global flag for graceful shutdown ──────────────────────────────── */
static volatile sig_atomic_t g_running = 1;

static void signal_handler(int sig)
{
    (void)sig;
    g_running = 0;
}

/* ── Helpers ────────────────────────────────────────────────────────── */

/* Return current time as a human-readable string (for logging). */
static const char *timestamp(void)
{
    static char buf[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

/* Check whether `name` ends with `suffix` (case-sensitive). */
static int has_suffix(const char *name, const char *suffix)
{
    size_t nlen = strlen(name);
    size_t slen = strlen(suffix);
    if (nlen < slen)
        return 0;
    return strcmp(name + nlen - slen, suffix) == 0;
}

/* Check if a path is a regular file. */
static int is_regular_file(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0)
        return 0;
    return S_ISREG(st.st_mode);
}

/*
 * libcurl write callback – discard the server response body.
 * Without this, curl prints the response to stdout.
 */
static size_t discard_response(void *ptr, size_t size, size_t nmemb,
                               void *userdata)
{
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

/* ── Core: upload a single file ─────────────────────────────────────── */

/*
 * Upload `filepath` to `server_url` as a multipart/form-data POST.
 * Returns 0 on success (HTTP 2xx), -1 on failure.
 */
static int upload_file(const char *filepath, const char *server_url)
{
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "[%s] ERROR  curl_easy_init() failed\n", timestamp());
        return -1;
    }

    /* Build the multipart form: field name "file", filename from path. */
    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, filepath);

    curl_easy_setopt(curl, CURLOPT_URL, server_url);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);          /* 30 s timeout  */
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);   /* 10 s connect  */

    CURLcode res = curl_easy_perform(curl);

    int ret = -1;
    if (res != CURLE_OK) {
        fprintf(stderr, "[%s] ERROR  upload failed (%s): %s\n",
                timestamp(), filepath, curl_easy_strerror(res));
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= HTTP_OK_MIN && http_code <= HTTP_OK_MAX) {
            ret = 0;   /* success */
        } else {
            fprintf(stderr, "[%s] ERROR  server returned HTTP %ld for %s\n",
                    timestamp(), http_code, filepath);
        }
    }

    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    return ret;
}

/* ── Core: scan directory and process each matching file ─────────────── */

static void scan_and_send(const char *watch_dir, const char *server_url)
{
    DIR *dir = opendir(watch_dir);
    if (!dir) {
        fprintf(stderr, "[%s] ERROR  cannot open directory '%s': %s\n",
                timestamp(), watch_dir, strerror(errno));
        return;
    }

    int sent_count = 0;
    int fail_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        /* Skip dotfiles and non-.txt entries. */
        if (entry->d_name[0] == '.')
            continue;
        if (!has_suffix(entry->d_name, FILE_EXTENSION))
            continue;

        /* Build full path. */
        char filepath[MAX_PATH_LEN];
        snprintf(filepath, sizeof(filepath), "%s/%s", watch_dir,
                 entry->d_name);

        /* Make sure it is a regular file. */
        if (!is_regular_file(filepath))
            continue;

        printf("[%s] INFO   sending %s\n", timestamp(), filepath);

        if (upload_file(filepath, server_url) == 0) {
            /* Upload succeeded → delete the file. */
            if (remove(filepath) == 0) {
                printf("[%s] INFO   sent & deleted %s\n",
                       timestamp(), filepath);
                sent_count++;
            } else {
                fprintf(stderr,
                        "[%s] WARN   sent but failed to delete %s: %s\n",
                        timestamp(), filepath, strerror(errno));
            }
        } else {
            fail_count++;
        }
    }

    closedir(dir);

    if (sent_count > 0 || fail_count > 0) {
        printf("[%s] INFO   cycle done — %d sent, %d failed\n",
               timestamp(), sent_count, fail_count);
    }
}

/* ── Entry point ────────────────────────────────────────────────────── */

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s <watch_directory> <server_url>\n"
                "Example:\n"
                "  %s /var/log/myapp http://192.168.1.10:8080/upload\n",
                argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    const char *watch_dir  = argv[1];
    const char *server_url = argv[2];

    /* Verify the watch directory exists. */
    {
        struct stat st;
        if (stat(watch_dir, &st) != 0 || !S_ISDIR(st.st_mode)) {
            fprintf(stderr, "ERROR: '%s' is not a valid directory.\n",
                    watch_dir);
            return EXIT_FAILURE;
        }
    }

    /* Install signal handlers for graceful shutdown (Ctrl-C / kill). */
    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);

    /* Global libcurl initialisation (once). */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    printf("[%s] INFO   file_sender started\n", timestamp());
    printf("[%s] INFO   watching : %s\n", timestamp(), watch_dir);
    printf("[%s] INFO   server   : %s\n", timestamp(), server_url);
    printf("[%s] INFO   filter   : *%s\n", timestamp(), FILE_EXTENSION);
    printf("[%s] INFO   interval : %d seconds\n", timestamp(),
           SCAN_INTERVAL_SEC);

    /* ── Main loop ──────────────────────────────────────────────── */
    while (g_running) {
        scan_and_send(watch_dir, server_url);

        /* Sleep in 1-second increments so we can react to signals. */
        for (int i = 0; i < SCAN_INTERVAL_SEC && g_running; i++)
            sleep(1);
    }

    curl_global_cleanup();

    printf("\n[%s] INFO   file_sender stopped (signal received)\n",
           timestamp());
    return EXIT_SUCCESS;
}
