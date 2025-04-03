#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include <QString>


class SecurityManager {
public:
  SecurityManager();
  static void setMasterPassword(const QString &password);
  static bool validateMasterPassword(const QString &input);
};

#endif // SECURITYMANAGER_H
