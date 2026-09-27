reggolyek

reggolyek is a Linux-focused keyboard-event logging research project written in C. It is intended for authorized security research, malware analysis, and controlled laboratory environments.

The project currently targets Linux systems, with a focus on Debian-based distributions, and is divided into two components:

main.c — Responsible for collecting keyboard events and writing the resulting log data locally.

sender.c — Responsible for transferring an existing log file to a configured research server.

Project Structure
reggolyek/
├── main.c       # Keyboard event logger
├── sender.c     # Log-file transfer component
└── README.md

Features

Written in C.

Designed for Linux environments.

Intended primarily for Debian-based distributions.

Separates keyboard-event collection from network transmission.

Stores captured event data in a local log file before transmission.

Can transfer the generated log file to a designated server.

Suitable for controlled malware-analysis and defensive-security research.

Intended Use

reggolyek should only be used on systems where you have explicit authorization to monitor keyboard input.

Appropriate use cases include:

Security research in an isolated virtual machine.

Malware-analysis laboratories.

Detection and incident-response testing.

Studying Linux input-event handling.

Testing defensive controls that detect unauthorized input monitoring.

Developing and validating security monitoring rules.

Do not deploy the software on computers, accounts, or networks without the owner's explicit permission.

Components
main.c

The logger component is responsible for collecting keyboard-related events and writing them to a local file for research purposes.

Its responsibilities are intentionally separated from network communication so that the logging and transmission portions can be analyzed independently.

sender.c

The sender component handles transmission of an already-generated log file to a designated research server.

Keeping this functionality separate makes it easier to test network-monitoring and detection mechanisms independently from the keyboard-event collection component.

Platform

The project is intended for:

Linux

Debian-based distributions

Compatibility may vary depending on the Linux kernel, input subsystem, permissions, and system configuration.

Security and Privacy

Keyboard logging can expose highly sensitive information, including passwords, authentication codes, private messages, and other confidential data.

For that reason:

Use reggolyek only in an isolated or explicitly authorized environment.

Never collect data from other users without informed authorization.

Avoid using real credentials or personal information during testing.

Protect generated log files as sensitive data.

Remove test logs after completing experiments.

Do not use the project to bypass security controls or obtain unauthorized information.

Research Disclaimer

This project is provided for educational and defensive-security research purposes. The author and contributors are not responsible for unauthorized, illegal, or harmful use of the software.

Before conducting experiments on a system that you do not personally own, obtain explicit authorization from the system owner and define the permitted scope of testing.

License

Add the project's chosen license here, for example:

MIT License


See the LICENSE file for the complete license terms.
