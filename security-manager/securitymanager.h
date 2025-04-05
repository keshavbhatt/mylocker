#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include <QString>

class SecurityManager {
public:
  SecurityManager();
  static void setMasterPassword(const QString &password);
  static bool validateMasterPassword(const QString &input);
  static bool getSessionKey(QByteArray &key, QByteArray &iv);
  static void clearSessionKey();

private:
  static void setSessionKey(const QByteArray &key, const QByteArray &iv);
  static inline QByteArray s_sessionKey;
  static inline QByteArray s_sessionIV;
};

#endif // SECURITYMANAGER_H
