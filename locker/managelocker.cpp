#include "managelocker.h"
#include "ui_managelocker.h"
#include "vaultcreationdialog.h"

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>

#include <icons/iconloader.h>
#include <icons/iconpickerdialog.h>

#include <theme/palette.h>

ManageLocker::ManageLocker(const QString &lockerDataDirPath, QWidget *parent)
    : QWidget(parent), m_lockerDataDirPath(lockerDataDirPath),
      ui(new Ui::ManageLocker) {
  ui->setupUi(this);

  ui->lockerNameLabel->setText(lockerDataDirPath);

  vaultListWidget = new VaultListWidget(this, true);
  vaultListWidget->loadFromDirectory(m_lockerDataDirPath);
  ui->vaultsLayout->addWidget(vaultListWidget);

  connect(vaultListWidget, &VaultListWidget::vaultSelectionChanged, this,
          &ManageLocker::updateVaultActionButtons);
  ui->createNewVaultButton->setIconSize(QSize(22, 22));
  ui->createNewVaultButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "add-circle-fill", Palette::iconSuccess()));
  connect(ui->createNewVaultButton, &QPushButton::clicked, this,
          &ManageLocker::createNewVault);

  ui->deleteSelectedVaultButton->setIconSize(QSize(22, 22));
  ui->deleteSelectedVaultButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "delete-bin-fill", Palette::iconError()));
  connect(ui->deleteSelectedVaultButton, &QPushButton::clicked, this,
          &ManageLocker::deleteSelectedVault);

  ui->editVaultButton->setIconSize(QSize(22, 22));
  ui->editVaultButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "edit-2-fill", Palette::iconWarning()));
  connect(ui->editVaultButton, &QPushButton::clicked, this,
          &ManageLocker::editSelectedVault);

  ui->closeButton->setIconSize(QSize(22, 22));
  ui->closeButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "close-circle-fill", Palette::iconError()));
  connect(ui->closeButton, &QPushButton::clicked, this, &ManageLocker::close);

  updateVaultActionButtons();
}

void ManageLocker::updateVaultActionButtons() {
  bool hasSelection = vaultListWidget->hasItemSelected();
  ui->deleteSelectedVaultButton->setEnabled(hasSelection);
  ui->editVaultButton->setEnabled(hasSelection);
}

ManageLocker::~ManageLocker() { delete ui; }

void ManageLocker::createNewVault() {
  VaultCreationDialog dlg(this);
  dlg.setModal(true);
  if (dlg.exec() != QDialog::Accepted)
    return;

  const QString newVaultName = dlg.vaultName().trimmed();
  if (newVaultName.isEmpty()) {
    QMessageBox::warning(this, tr("Invalid Name"),
                         tr("Vault name cannot be empty."));
    return;
  }

  QDir dir(m_lockerDataDirPath);
  QString newVaultPath = dir.filePath(newVaultName);

  if (QDir(newVaultPath).exists()) {
    QMessageBox::warning(this, tr("Vault Exists"),
                         tr("A vault with this name already exists."));
    return;
  }

  if (!dir.mkdir(newVaultName)) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Failed to create vault directory."));
    return;
  }

  QSettings metaSettings(newVaultPath + "/.vault.meta", QSettings::IniFormat);
  metaSettings.setValue("Vault/icon", dlg.selectedIcon());
  metaSettings.setValue("Vault/color", dlg.selectedColor().name());

  vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
}

void ManageLocker::editSelectedVault() {
  QString selectedVault = vaultListWidget->selectedVaultName();
  if (selectedVault.isEmpty()) {
    QMessageBox::warning(this, tr("No Vault Selected"),
                         tr("Please select a vault to edit."));
    return;
  }

  QString vaultPath = QDir(m_lockerDataDirPath).filePath(selectedVault);
  QSettings metaSettings(vaultPath + "/.vault.meta", QSettings::IniFormat);
  QString icon = metaSettings.value("Vault/icon").toString();
  QColor color(metaSettings.value("Vault/color").toString());

  VaultCreationDialog dlg(this);
  dlg.setWindowTitle(tr("Edit Vault"));
  dlg.setVaultName(selectedVault);
  dlg.setIconAndIconColor(color, icon);

  if (dlg.exec() == QDialog::Accepted) {
    metaSettings.setValue("Vault/icon", dlg.selectedIcon());
    metaSettings.setValue("Vault/color", dlg.selectedColor().name());
    vaultListWidget->loadFromDirectory(m_lockerDataDirPath);
  }
}

void ManageLocker::deleteSelectedVault() {
  QString selectedVault = vaultListWidget->selectedVaultName();
  if (selectedVault.isEmpty()) {
    QMessageBox::warning(this, tr("No Vault Selected"),
                         tr("Please select a vault to delete."));
    return;
  }

  QMessageBox::StandardButton confirm = QMessageBox::question(
      this, tr("Delete Vault"),
      tr("Are you sure you want to delete the vault '%1'?").arg(selectedVault),
      QMessageBox::Yes | QMessageBox::No);

  if (confirm == QMessageBox::Yes) {
    QDir vaultDir(m_lockerDataDirPath + "/" + selectedVault);
    if (vaultDir.removeRecursively()) {
      vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
    } else {
      QMessageBox::critical(this, tr("Error"),
                            tr("Failed to delete vault directory."));
    }
  }
}
