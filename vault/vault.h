#ifndef VAULT_H
#define VAULT_H

#include <QColor>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QString>

#include <theme/palette.h>

struct VaultMeta {
  QString icon;
  QColor color;

  VaultMeta() = default;

  VaultMeta(const QString &iconName, const QColor &colorValue)
      : icon(iconName), color(colorValue) {}

  bool isValid() const { return !icon.isEmpty() && color.isValid(); }
};

class Vault {
public:
  explicit Vault(const QString &path);

  QString name() const;
  QString path() const;
  QString lockerDirPath() const;

  static QString defaultIcon();
  static QColor defaultColor();
  static void saveNewVaultMeta(const QString &vaultPath, const VaultMeta &meta);
  static VaultMeta loadMeta(const QString &vaultPath);

  VaultMeta loadMeta() const;
  void saveMeta(const VaultMeta &meta) const;

private:
  QString m_path;
};

#endif // VAULT_H
