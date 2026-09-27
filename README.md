⌨️ reggolyek
🐧 Linux Keyboard-Event Logging Research Project
<p align="center"> <b>🔬 Security Research • 🛡️ Defensive Testing • 🐧 Linux</b> </p> <p align="center"> <img src="https://img.shields.io/badge/Platform-Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black"> <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c"> <img src="https://img.shields.io/badge/Focus-Security%20Research-8A2BE2?style=for-the-badge"> <img src="https://img.shields.io/badge/Target-Debian%20Based-A81D33?style=for-the-badge&logo=debian"> </p>
🌟 About

reggolyek is a Linux-based keyboard-event logging research project written in C.

The project is designed for authorized security research, malware analysis, defensive testing, and isolated laboratory environments.

Its architecture separates local event logging from network transmission, making the two components easier to study independently.

⚠️ Important: Keyboard logging can expose extremely sensitive information. Use this project only on systems where you have explicit authorization.

🧩 Project Architecture
                    ┌─────────────────────┐
                    │      reggolyek       │
                    │   Linux Research    │
                    │       Project       │
                    └──────────┬──────────┘
                               │
                ┌──────────────┴──────────────┐
                │                             │
                ▼                             ▼
       ┌─────────────────┐          ┌─────────────────┐
       │     main.c      │          │    sender.c     │
       │                 │          │                 │
       │ ⌨️ Event Logger │          │ 📡 Log Sender   │
       │                 │          │                 │
       │ Local logging   │          │ File transfer   │
       └────────┬────────┘          └────────┬────────┘
                │                            │
                ▼                            ▼
          📄 Local Log                 🌐 Research
             File                      Server

📁 Repository Structure
reggolyek/
│
├── 📄 main.c
│   └── ⌨️ Keyboard-event logging component
│
├── 📄 sender.c
│   └── 📡 Log-file transmission component
│
├── 📄 README.md
│   └── 📚 Project documentation
│
└── 📄 LICENSE
    └── ⚖️ License information

⚙️ Components
⌨️ main.c

The primary logging component.

Responsibilities include:

🐧 Handling Linux keyboard-event input.

📝 Writing collected event information to a local log.

🔬 Providing a component that can be analyzed independently during security research.

📡 sender.c

The network-transfer component.

Responsibilities include:

📄 Reading an existing research log file.

🌐 Sending the file to a designated research server.

🧪 Providing a separate component for studying network-based detection.

🖥️ Supported Environment
Environment	Status
🐧 Linux	✅ Supported
🔴 Debian	✅ Target
🟠 Ubuntu	✅ Debian-based
🟣 Linux Mint	✅ Debian/Ubuntu-based
🟢 Other Debian-based distributions	⚠️ May work
🪟 Windows	❌ Not supported
🍎 macOS	❌ Not supported

Compatibility can depend on the Linux kernel, input subsystem, permissions, and system configuration.

🔬 Intended Research Uses

reggolyek can be used in controlled environments for:

🧪 Malware-analysis laboratories

🛡️ Defensive-security research

🔍 Detection-rule development

📊 Security monitoring experiments

🐧 Linux input-subsystem research

🚨 Testing detection of unauthorized input monitoring

🎓 Educational security experimentation

🔐 Privacy & Safety

Keyboard-event logging can potentially capture:

🔑 Passwords
💳 Financial information
📧 Private messages
🔢 Authentication codes
📄 Confidential documents
👤 Personally identifiable information


Therefore, research environments should use synthetic test data rather than real credentials or personal information.

🛡️ Recommended precautions

🔒 Use an isolated virtual machine or dedicated test system.

👤 Obtain explicit authorization before monitoring a system.

🧪 Use test accounts and synthetic data.

📁 Treat generated logs as sensitive information.

🗑️ Delete test data after experiments.

🚫 Never deploy the software covertly on systems you do not own or administer.

🧠 Research Architecture

The separation between the two C files provides a simple research architecture:

        ┌───────────────┐
        │   Linux Input │
        │     Events    │
        └───────┬───────┘
                │
                ▼
        ┌───────────────┐
        │    main.c     │
        │   ⌨️ Logger   │
        └───────┬───────┘
                │
                ▼
        ┌───────────────┐
        │   Local Log   │
        │      📄       │
        └───────┬───────┘
                │
                ▼
        ┌───────────────┐
        │   sender.c    │
        │   📡 Sender   │
        └───────┬───────┘
                │
                ▼
        ┌───────────────┐
        │ Research/Test │
        │    Server     │
        │      🌐       │
        └───────────────┘

🚀 Development

The project is intentionally divided into small components so researchers can inspect and test each part independently.

main.c
  │
  └── Event collection
          │
          ▼
      Local logging
          │
          ▼
      sender.c
          │
          └── Network transmission


Build and configuration instructions should be adapted to the specific authorized laboratory environment in which the project is being tested.

🧪 Testing Environment

For safe experimentation, a recommended setup is:

┌──────────────────────────────┐
│       🧪 Test Machine        │
│                              │
│       Debian / Ubuntu        │
│             │                │
│             ▼                │
│        ┌─────────┐           │
│        │reggolyek│           │
│        └────┬────┘           │
│             │                │
│             ▼                │
│       Synthetic Data         │
└─────────────┬────────────────┘
              │
              ▼
       🌐 Isolated Test
           Server


Avoid testing with real passwords, authentication tokens, financial information, or other sensitive data.

⚖️ Disclaimer

reggolyek is intended for authorized security research and educational purposes only.

The user is responsible for obtaining appropriate authorization before deploying or testing the software.

The project should not be used to secretly monitor other people, obtain credentials, invade privacy, or access systems without authorization.

📜 License

Add your chosen license to the repository:

LICENSE


For example:

MIT License


See the LICENSE file for the complete terms.

<p align="center">
⌨️ reggolyek

🐧 Linux • 🔬 Research • 🛡️ Security • 📡 Networking

Made for controlled security research environments.

</p>
