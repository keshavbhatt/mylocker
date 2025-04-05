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
  if (inputHash != storedHash) {
    return false;
  }

  // Derive key + IV from input password and store it for session use
  QByteArray key, iv;
  if (!Encryptor::deriveKeyAndIV(input, storedSalt, key, iv)) {
    return false;
  }

  SecurityManager::setSessionKey(key, iv);
  return true;
}

void SecurityManager::clearSessionKey() {
  volatile char *ptr = s_sessionKey.data();
  for (int i = 0; i < s_sessionKey.size(); ++i)
    ptr[i] = 0;

  ptr = s_sessionIV.data();
  for (int i = 0; i < s_sessionIV.size(); ++i)
    ptr[i] = 0;

  s_sessionKey.clear();
  s_sessionIV.clear();
}

void SecurityManager::setSessionKey(const QByteArray &key,
                                    const QByteArray &iv) {
  s_sessionKey = key;
  s_sessionIV = iv;
}

bool SecurityManager::getSessionKey(QByteArray &key, QByteArray &iv) {
  if (s_sessionKey.isEmpty() || s_sessionIV.isEmpty())
    return false;

  key = s_sessionKey;
  iv = s_sessionIV;
  return true;
}
