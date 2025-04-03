#include "securitymanager.h"
#include <encryptor/encryptor.h>
#include <settings/settingsmanager.h>

SecurityManager::SecurityManager() {}

void SecurityManager::setMasterPassword(const QString &password) {
  QByteArray salt = Encryptor::generateSalt();
  QByteArray hashed = Encryptor::hashPassword(password, salt);

  SettingsManager::instance().setValue("security/master_hash",
                                       hashed.toBase64());
  SettingsManager::instance().setValue("security/master_salt", salt.toBase64());

  SettingsManager::instance().setValue("security/logout_required", true);
}

bool SecurityManager::validateMasterPassword(const QString &input) {
  QString storedHashBase64 =
      SettingsManager::instance().getValue("security/master_hash").toString();
  QString storedSaltBase64 =
      SettingsManager::instance().getValue("security/master_salt").toString();

  if (storedHashBase64.isEmpty() || storedSaltBase64.isEmpty()) {
    return false;
  }

  QByteArray storedHash = QByteArray::fromBase64(storedHashBase64.toUtf8());
  QByteArray storedSalt = QByteArray::fromBase64(storedSaltBase64.toUtf8());

  QByteArray inputHash = Encryptor::hashPassword(input, storedSalt);
  return inputHash == storedHash;
}
