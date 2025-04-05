# MyLocker

**MyLocker** is a secure personal vault and offline password manager built using Qt 5. It stores all your sensitive data—like passwords, login credentials, and notes—in an encrypted local file, protected by a master password only you know.

---

## 🔐 Features

### Password Manager

- **Master Password Protection** – All password entries are secured with a master password only you know.
- **End-to-End Encryption** – Password data is encrypted locally using AES. Nothing is stored in plain text.
- **Clipboard Auto-Clear** – Automatically clears copied passwords from the clipboard after a short duration.
- **Search & Filter** – Quickly find password entries with real-time filtering.
- **Edit & Duplicate Entries** – Update or clone entries to speed up creation.
- **Custom Storage Path** – Choose where the encrypted password vault is saved.
- **Auto-Lock** – Automatically locks the vault after a period of inactivity.

### General

- **Local-Only** – No cloud sync, no telemetry. Your data stays on your device.

> **Note:** Support for **secure notes** and other **vault content** is planned for future.

---

## 📷 Screenshots

> _Coming soon..._

---

## 🛠️ Build Instructions

### Requirements

- Qt 5.15 or above (with `core`, `gui`, `widgets` modules)
- qmake
- C++17-compatible compiler
- OpenSSL development libraries

### Linux (or Unix-like) Build

```bash
git clone https://github.com/keshavbhatt/mylocker.git
cd mylocker
qmake
make
./mylocker
```
---

## 📄 License

Licensed under the [MIT](https://github.com/keshavbhatt/mylocker/blob/main/LICENSE) License.
Feel free to modify, fork, and contribute!

## 🤝 Contributing

Pull requests and feedback are welcome! Please open issues for bugs or feature requests.


Built with 💙 using **[Qt](https://www.qt.io/)**
