#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

// TODO TRASH THIS SHIT
namespace Config {

inline QString STORAGE_FILE =
    "passwords.dat"; // Default storage path (modifiable at runtime)

// Encryption settings
const QByteArray ENCRYPTION_KEY =
    "0123456789abcdef0123456789abcdef";              // 32 bytes
const QByteArray ENCRYPTION_IV = "0123456789abcdef"; // 16 bytes

} // namespace Config

#endif // CONFIG_H
