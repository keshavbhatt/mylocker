#include "vault.h"

Vault::Vault(const QString &path) : m_path(path) {}

QString Vault::name() const { return QFileInfo(m_path).fileName(); }

QString Vault::path() const { return m_path; }

QString Vault::defaultIcon() { return "dv-line"; }

QColor Vault::defaultColor() { return Palette::iconPrimary(); }

QString Vault::lockerDirPath() const {
  return QFileInfo(m_path).absolutePath();
}

VaultMeta Vault::loadMeta() const {
  QSettings metaSettings(m_path + QDir::separator() + ".vault.meta",
                         QSettings::IniFormat);

  VaultMeta meta;
  meta.icon = metaSettings.value("Vault/icon", defaultIcon()).toString();
  meta.color = QColor(
      metaSettings.value("Vault/color", defaultColor().name()).toString());
  return meta;
}

VaultMeta Vault::loadMeta(const QString &vaultPath) {
  QSettings metaSettings(vaultPath + QDir::separator() + ".vault.meta",
                         QSettings::IniFormat);

  VaultMeta meta;
  meta.icon = metaSettings.value("Vault/icon", defaultIcon()).toString();
  meta.color = QColor(
      metaSettings.value("Vault/color", defaultColor().name()).toString());
  return meta;
}

void Vault::saveMeta(const VaultMeta &meta) const {
  QSettings metaSettings(m_path + QDir::separator() + ".vault.meta",
                         QSettings::IniFormat);
  metaSettings.setValue("Vault/icon", meta.icon);
  metaSettings.setValue("Vault/color", meta.color.name());
}

void Vault::saveNewVaultMeta(const QString &vaultPath, const VaultMeta &meta) {
  QSettings metaSettings(vaultPath + QDir::separator() + ".vault.meta",
                         QSettings::IniFormat);
  metaSettings.setValue("Vault/icon", meta.icon);
  metaSettings.setValue("Vault/color", meta.color.name());
}
