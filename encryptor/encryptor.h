#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QString>
#include <openssl/evp.h>

class Encryptor {
public:
  // TODO: save this along with master_hash and salt in locker settings
  static constexpr int DEFAULT_ITERATION_COUNT = 100000; // do not change it

  static QByteArray encrypt(const QByteArray &plaintext, const QByteArray &key,
                            const QByteArray &iv);
  static QByteArray decrypt(const QByteArray &ciphertext, const QByteArray &key,
                            const QByteArray &iv);

  static QByteArray generateSalt(int length = 16);
  static QByteArray hashPassword(const QString &password,
                                 const QByteArray &salt,
                                 int iterations = DEFAULT_ITERATION_COUNT);
  static bool deriveKeyAndIV(const QString &password, const QByteArray &salt,
                             QByteArray &outKey, QByteArray &outIV,
                             int iterations = DEFAULT_ITERATION_COUNT);
};

#endif // ENCRYPTOR_H
