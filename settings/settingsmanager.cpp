#include "settingsmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

SettingsManager::SettingsManager() {
  // Default to application settings path
  // TODO: change this to standardpath config path
  storageFile = QCoreApplication::applicationDirPath() + "/settings.ini";
  settings = new QSettings(storageFile, QSettings::IniFormat);
}

SettingsManager::~SettingsManager() { delete settings; }

SettingsManager &SettingsManager::instance() {
  static SettingsManager instance;
  return instance;
}

void SettingsManager::setValue(const QString &key, const QVariant &value) {
  QMutexLocker locker(&mutex);
  settings->setValue(key, value);
  settings->sync();
}

QVariant SettingsManager::getValue(const QString &key,
                                   const QVariant &defaultValue) const {
  QMutexLocker locker(&mutex);
  return settings->value(key, defaultValue);
}

void SettingsManager::setStorageFile(const QString &filePath) {
  QMutexLocker locker(&mutex);
  storageFile = filePath;
  delete settings;
  settings = new QSettings(storageFile, QSettings::IniFormat);
}

QString SettingsManager::getStorageFile() const {
  QMutexLocker locker(&mutex);
  return storageFile;
}

QString SettingsManager::getPasswordStorageFilePath() const {
  const QString baseDirPath = instance()
                                  .getValue("paths/passwordmanager-file-path",
                                            QStandardPaths::writableLocation(
                                                QStandardPaths::DataLocation))
                                  .toString();

  QDir dir;
  if (!dir.exists(baseDirPath)) {
    dir.mkpath(baseDirPath);
  }

  QString fullPath = baseDirPath + QDir::separator() + "passwords.dat";
  return fullPath;
}
