# MyLocker

**MyLocker** is a secure personal locker built using Qt. It organizes your data into encrypted vaults stored inside a master-password protected **Locker**. MyLocker is completely local – your data never leaves your device (unless you choose to sync via Git).

---

## 🚀 Application Features

- **Locker-Based Architecture** – Organize multiple encrypted vaults within a secure portable Locker.
- **Master Password Authentication** – Unlock all your data using a single master password.
- **Multiple Vaults** – Manage several vaults independently inside a single Locker.
- **Locker Management** – Create or delete vaults within existing lockers.
- **Configurable Storage Path** – Choose where your locker and vault data is saved.
- **Auto-Lock** – Automatically locks the application after a period of inactivity for enhanced security.
- **Local-Only by Default** – No cloud sync or telemetry. Your data stays on your device unless Git sync is enabled.
- **Optional Git Sync** – Link a Git repository to your Locker to sync all vaults securely.

---

## 🔐 Password Manager

 - **End-to-End Encryption** – All password entries are securely encrypted locally using AES-256 and are stored inside your encrypted vaults.
- **Clipboard Auto-Clear** – Copied passwords are automatically cleared from the clipboard after a short time.
- **Search & Filter** – Instantly find entries with live filtering and keyword matching.
- **Edit & Duplicate Entries** – Easily update or duplicate existing records for convenience.

---

## 📝 Notes Module

- **Encrypted Notes** – Add simple plaintext notes which are securely encrypted locally using AES-256 and are stored inside your encrypted vaults.
- **Search & Filter** – Quickly locate notes with live filtering support.
- **Edit & Duplicate Entries** – Easily update or duplicate existing records for convenience.

---

## 💡 Coming Soon

- Rich Text Notes with Markdown Support
- Improved Theme/UX and Accessibility Options

---

## 📷 Screenshots

> _Coming soon..._

🎥 Watch a quick walkthrough of the application's capabilities (Click the image below to watch on Youtube):

[![YouTube](http://i.ytimg.com/vi/_sv8ORJZ6d0/hqdefault.jpg)](https://www.youtube.com/watch?v=_sv8ORJZ6d0)

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
