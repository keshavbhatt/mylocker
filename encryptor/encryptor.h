#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QString>
#include <openssl/evp.h>

class Encryptor {
public:
  static QByteArray encrypt(const QString &plaintext, const QByteArray &key,
                            const QByteArray &iv);
  static QString decrypt(const QByteArray &ciphertext, const QByteArray &key,
                         const QByteArray &iv);

  static QByteArray generateSalt(int length = 16);
  static QByteArray hashPassword(const QString &password,
                                 const QByteArray &salt,
                                 int iterations = 100000);
};

#endif // ENCRYPTOR_H
