#include "vaultlistwidget.h"

#include <QDir>
#include <QFileInfoList>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

VaultListWidget::VaultListWidget(QWidget *parent)
    : QWidget(parent), vaultListWidget(new QListWidget(this)) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  m_vaultLabel = new QLabel("");
  layout->addWidget(m_vaultLabel);
  layout->addWidget(vaultListWidget);
  layout->setContentsMargins(0, 0, 0, 0);

  connect(vaultListWidget, &QListWidget::currentTextChanged, this,
          &VaultListWidget::vaultSelectionChanged);
}

void VaultListWidget::loadFromDirectory(const QString &directoryPath) {
  vaultListWidget->clear();
  m_vaultLabel->setText(QString("Vaults from: %1").arg(directoryPath));
  QDir dir(directoryPath);
  const QFileInfoList entries =
      dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

  // Add all entries to the list widget
  for (const QFileInfo &entry : entries) {
    vaultListWidget->addItem(entry.fileName());
  }

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
