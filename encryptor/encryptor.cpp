#include "encryptor.h"
#include <QByteArray>
#include <openssl/rand.h>

QByteArray Encryptor::encrypt(const QString &plaintext, const QByteArray &key,
                              const QByteArray &iv) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  QByteArray ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH,
                        Qt::Uninitialized);

  int len = 0, ciphertext_len = 0;
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                     (unsigned char *)key.data(), (unsigned char *)iv.data());
  EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext.data(), &len,
                    (unsigned char *)plaintext.toUtf8().data(),
                    plaintext.size());
  ciphertext_len += len;
  EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext.data() + len, &len);
  ciphertext_len += len;
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext.left(ciphertext_len);
}

QString Encryptor::decrypt(const QByteArray &ciphertext, const QByteArray &key,
                           const QByteArray &iv) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  QByteArray plaintext(ciphertext.size(), Qt::Uninitialized);

  int len = 0, plaintext_len = 0;
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                     (unsigned char *)key.data(), (unsigned char *)iv.data());
  EVP_DecryptUpdate(ctx, (unsigned char *)plaintext.data(), &len,
                    (unsigned char *)ciphertext.data(), ciphertext.size());
  plaintext_len += len;
  EVP_DecryptFinal_ex(ctx, (unsigned char *)plaintext.data() + len, &len);
  plaintext_len += len;
  EVP_CIPHER_CTX_free(ctx);

  return QString::fromUtf8(plaintext.left(plaintext_len));
}

QByteArray Encryptor::generateSalt(int length) {
  QByteArray salt(length, Qt::Uninitialized);
  if (RAND_bytes(reinterpret_cast<unsigned char *>(salt.data()), length) != 1) {
    return QByteArray();
  }
  return salt.toBase64();
}

QByteArray Encryptor::hashPassword(const QString &password,
                                   const QByteArray &salt, int iterations) {
  QByteArray key(32, Qt::Uninitialized); // 256-bit key
  if (PKCS5_PBKDF2_HMAC_SHA1(
          password.toUtf8().data(), password.size(),
          reinterpret_cast<const unsigned char *>(salt.data()), salt.size(),
          iterations, key.size(),
          reinterpret_cast<unsigned char *>(key.data())) == 0) {
    return QByteArray();
  }
  return key.toBase64();
}
