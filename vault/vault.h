#ifndef VAULT_H
#define VAULT_H

#include <QColor>
#include <QFileInfo>
#include <QSettings>
#include <QString>

struct VaultMeta {
  QString icon;
  QColor color;
};

class Vault {
public:
  explicit Vault(const QString &path);

  QString name() const;
  QString path() const;
  QString lockerDirPath() const;

  static QString defaultIcon();
  static QColor defaultColor();

  VaultMeta loadMeta() const;
  void saveMeta(const VaultMeta &meta);

private:
  QString m_path;
};

#endif // VAULT_H
