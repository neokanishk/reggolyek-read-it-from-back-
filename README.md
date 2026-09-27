🐧 Linux Keyboard-Event Logging Research Project
<p align="center"> <b>🔬 Security Research • 🛡️ Defensive Testing • 🐧 Linux</b> </p> <p align="center"> <img src="https://img.shields.io/badge/Platform-Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black"> <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c"> <img src="https://img.shields.io/badge/Target-Debian%20Based-A81D33?style=for-the-badge&logo=debian"> <img src="https://img.shields.io/badge/Purpose-Security%20Research-8A2BE2?style=for-the-badge"> </p>

</p>
🌟 About

reggolyek is a small Linux-based keyboard-event logging research project written in C.

The project is divided into two components:

⌨️ main.c — Keyboard-event logger
📡 sender.c — Log-file sender

This separation allows the logging and network-transfer components to be studied independently in an authorized research environment.

🧩 How It Works
⌨️ 1. Logger

main.c handles the local keyboard-event logging portion of the project.

It produces a local test log that can subsequently be inspected or processed.

⬇️

📄 2. Local Log

The collected test data is stored locally.

⬇️

📡 3. Sender

sender.c handles transmission of an existing test log to the configured research server.

⬇️

🌐 4. Research Server

The test log reaches the designated server for analysis.

📁 Project Structure
File	Description
main.c	⌨️ Keyboard-event logging component
sender.c	📡 Log-file transmission component
README.md	📚 Project documentation
LICENSE	⚖️ License information
🖥️ Platform
Supported

🐧 Linux

🔴 Debian

🟠 Ubuntu

🟢 Linux Mint

🟣 Other Debian-based distributions

Not Supported

🪟 Windows

🍎 macOS

Compatibility can vary depending on the Linux kernel, input subsystem, permissions, and system configuration.

🛠️ Requirements

A Debian-based development environment with a C compiler is recommended.

Install the basic build tools:

sudo apt update
sudo apt install build-essential


The implementation may require additional development libraries depending on the APIs used by the source code.

🚀 Usage
🔨 Build

Compile the logger:

gcc main.c -o reggolyek


Compile the sender:

gcc sender.c -o reggolyek-sender


Your implementation may require additional compiler flags or libraries.

⌨️ Run the Logger

Start the logger inside an authorized test environment:

./reggolyek


The logger records keyboard events into its configured local test log.

Workflow

⌨️ Test Input
↓
main.c
↓
📄 Local Test Log

Use synthetic test input rather than real passwords, authentication codes, or private information.

📡 Send a Test Log

After generating a test log:

./reggolyek-sender <test-log-file>

Workflow

📄 Test Log
↓
sender.c
↓
🌐 Authorized Research Server

The server configuration should be appropriate for your isolated research environment.

🧪 Example Research Workflow
Step	Action
01	🧪 Create an isolated Linux test environment
02	🔨 Build the project
03	⌨️ Generate synthetic keyboard input
04	📄 Inspect the generated test log
05	📡 Transfer the test log
06	🔍 Analyze endpoint/network telemetry
07	🗑️ Remove test data
🔬 Security Research

reggolyek can be used in controlled environments to study:

🔍 Linux process monitoring

🐧 Input-device access

📁 Suspicious log-file creation

🌐 Unexpected network connections

🚨 Endpoint detection

📊 Network telemetry

🛡️ Security monitoring

🔬 Malware-analysis techniques

🔐 Privacy & Safety

Keyboard logging can potentially expose extremely sensitive information.

✅ Recommended

Use an isolated test machine.

Use synthetic test data.

Obtain explicit authorization.

Protect generated logs.

Delete test data after experiments.

❌ Avoid

Unauthorized monitoring.

Collecting real credentials.

Capturing private information.

Covert deployment.

Bypassing security controls.

🎯 Project Focus
<p align="center">
🔬 Research	🐧 Linux	🛡️ Defense
Security experimentation	Debian-based systems	Detection research
</p>

The project is intended to provide a compact environment for studying the relationship between local input monitoring and network activity.

📜 License

Place your chosen license in the repository as:

LICENSE


For example:

MIT License


See the LICENSE file for the complete terms.

⚠️ Disclaimer

reggolyek is intended for authorized security research and educational purposes only.

Use it only on systems where you have explicit permission to perform monitoring and testing.

<p align="center">
⌨️ reggolyek

🐧 Linux · 🔬 Research · 🛡️ Security · 📡 Networking

<sub>Built for controlled security research environments.</sub>

</p>
