#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QMutex>
#include <QSettings>

class SettingsManager {
public:
  static SettingsManager &instance();

  void setValue(const QString &key, const QVariant &value);
  QVariant getValue(const QString &key,
                    const QVariant &defaultValue = QVariant()) const;

  void setStorageFile(const QString &filePath);
  QString getStorageFile() const;

  QString getPasswordStorageFilePath() const;

  QString getNoteStorageFilePath() const;

  int getClipboardClearDelay() const;

private:
  SettingsManager();
  ~SettingsManager();

  SettingsManager(const SettingsManager &) = delete;
  SettingsManager &operator=(const SettingsManager &) = delete;

  mutable QMutex mutex;
  QSettings *settings;
  QString storageFile;
  void updateStoragePath();
};

#endif // SETTINGSMANAGER_H
