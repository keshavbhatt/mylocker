#include "vaultlistwidget.h"

#include "icons/iconloader.h"
#include <QDir>
#include <QFileInfoList>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>
#include <theme/palette.h>

VaultListWidget::VaultListWidget(QWidget *parent, const bool &embded)
    : QWidget(parent), vaultListWidget(new QListWidget(this)),
      m_isEmbeded(embded) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  m_vaultLabel = new QLabel("");
  layout->addWidget(m_vaultLabel);

  if (m_isEmbeded) {
    m_vaultLabel->setVisible(false);
  }

  vaultListWidget->setIconSize(QSize(32, 32));
  layout->addWidget(vaultListWidget);
  layout->setContentsMargins(0, 0, 0, 0);

  connect(vaultListWidget, &QListWidget::currentTextChanged, this,
          &VaultListWidget::vaultSelectionChanged);
}

bool VaultListWidget::hasItemSelected() {
  return vaultListWidget->currentRow() >= 0;
}

void VaultListWidget::loadFromDirectory(const QString &directoryPath) {
  vaultListWidget->clear();
  m_vaultLabel->setText(QString("Vaults from: %1").arg(directoryPath));
  QDir dir(directoryPath);
  const QFileInfoList entries =
      dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

  for (const QFileInfo &entry : entries) {
    QListWidgetItem *item = new QListWidgetItem(entry.fileName());

    QSettings meta(entry.filePath() + QDir::separator() + ".vault.meta",
                   QSettings::IniFormat);
    QString iconName = meta.value("Vault/icon").toString();
    QString iconColor = meta.value("Vault/color").toString();

    if (!iconName.isEmpty()) {
      QColor color =
          iconColor.isEmpty() ? Palette::iconDefault() : QColor(iconColor);
      QIcon icon = Utils::IconLoader::loadColoredIcon(iconName, color);
      item->setIcon(icon);
    }

    vaultListWidget->addItem(item);
  }

  if (m_isEmbeded == false) {
    selectCurrentListItemFromSettings();
  }
}

void VaultListWidget::selectCurrentListItemFromSettings() {
  if (vaultListWidget->count() > 0) {
    QSettings settings;
    QString lastOpenedVaultName =
        settings.value("lastOpenedVaultName").toString();
    if (!lastOpenedVaultName.isNull() || !lastOpenedVaultName.isEmpty()) {
      bool found = false;
      for (int i = 0; i < vaultListWidget->count(); ++i) {
        if (vaultListWidget->item(i)->text() == lastOpenedVaultName) {
          vaultListWidget->setCurrentRow(i);
          found = true;
          break;
        }
      }

      if (!found) {
        vaultListWidget->setCurrentRow(0);
      }
    }
  }
}

QString VaultListWidget::selectedVaultName() const {
  QListWidgetItem *item = vaultListWidget->currentItem();
  return item ? item->text() : QString();
}
