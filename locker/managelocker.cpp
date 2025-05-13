#include "managelocker.h"
#include "ui_managelocker.h"

#include <vault/vault.h>
#include <vault/vaultmanager.h>

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

  // Sync controls
  m_syncController = new SyncController(m_lockerDataDirPath, this);

  bool syncEnabled = m_syncController->isSyncEnabled();
  QString remoteUrl = m_syncController->remoteUrl();
  bool isGitRepo = m_syncController->isGitRepo();

  bool isValidSyncState = syncEnabled && !remoteUrl.isEmpty() && isGitRepo;
  ui->gitClearRemoteUrl->setEnabled(isValidSyncState);
  ui->gitRemoteUrlLineEdit->setEnabled(isValidSyncState);

  ui->gitSyncCheckBox->setChecked(isValidSyncState);
  ui->gitRemoteUrlLineEdit->setReadOnly(true);
  ui->gitRemoteUrlLineEdit->setText(!remoteUrl.isEmpty() ? remoteUrl : "-");

  connect(ui->gitClearRemoteUrl, &QPushButton::clicked, this, [=]() {
    m_syncController->disableSync();
    ui->gitSyncCheckBox->setChecked(false);
    ui->gitRemoteUrlLineEdit->setText("-");
  });

  connect(ui->gitSyncCheckBox, &QCheckBox::toggled, this,
          &ManageLocker::onGitSyncToggled);

  updateVaultActionButtons();
}

void ManageLocker::onGitSyncToggled(bool checked) {
  ui->gitClearRemoteUrl->setEnabled(checked);
  ui->gitRemoteUrlLineEdit->setEnabled(checked);

  if (checked) {
    QString remoteUrl = m_syncController->remoteUrl();

    if (remoteUrl.isEmpty()) {
      remoteUrl = QInputDialog::getText(this, tr("Git Remote URL"),
                                        tr("Enter remote repository URL:"));
      if (remoteUrl.isEmpty()) {
        ui->gitSyncCheckBox->setChecked(false);
        return;
      }
    }

    if (!m_syncController->enableSync(remoteUrl)) {
      QMessageBox::critical(this, tr("Sync Error"),
                            tr("Failed to initialize Git repository.\n"
                               "Please check your remote URL and try again."));
      ui->gitSyncCheckBox->setChecked(false);
      ui->gitRemoteUrlLineEdit->setText("-");
      return;
    }

    ui->gitRemoteUrlLineEdit->setText(m_syncController->remoteUrl());

  } else {
    m_syncController->disableSync(false);
  }
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

  VaultMeta vMeta(dlg.selectedIcon(), dlg.selectedColor());
  Vault::saveNewVaultMeta(newVaultPath, vMeta);

  vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
}

void ManageLocker::editSelectedVault() {
  QString selectedVault = vaultListWidget->selectedVaultName();
  if (selectedVault.isEmpty()) {
    QMessageBox::warning(this, tr("No Vault Selected"),
                         tr("Please select a vault to edit."));
    return;
  }

  QString oldVaultPath = QDir(m_lockerDataDirPath).filePath(selectedVault);

  VaultMeta meta = Vault::loadMeta(oldVaultPath);

  VaultCreationDialog dlg(this);
  dlg.setWindowTitle(tr("Edit Vault"));
  dlg.setVaultName(selectedVault);
  dlg.setIconAndIconColor(meta.color, meta.icon);

  if (dlg.exec() == QDialog::Accepted) {
    QString newVaultName = dlg.vaultName().trimmed();

    if (newVaultName.isEmpty()) {
      QMessageBox::warning(this, tr("Invalid Name"),
                           tr("Vault name cannot be empty."));
      return;
    }

    if (newVaultName != selectedVault) {
      QString newVaultPath = QDir(m_lockerDataDirPath).filePath(newVaultName);
      if (QFileInfo::exists(newVaultPath)) {
        QMessageBox::critical(this, tr("Name Conflict"),
                              tr("A vault with the new name already exists."));
        return;
      }

      QDir lockerDir(m_lockerDataDirPath);
      if (!lockerDir.rename(selectedVault, newVaultName)) {
        QMessageBox::critical(this, tr("Rename Failed"),
                              tr("Failed to rename the vault folder."));
        return;
      }
    }

    QString updatedVaultPath = QDir(m_lockerDataDirPath).filePath(newVaultName);

    VaultMeta updatedMeta(dlg.selectedIcon(), dlg.selectedColor());
    Vault::saveNewVaultMeta(updatedVaultPath, updatedMeta);

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
    QDir vaultDir(m_lockerDataDirPath + QDir::separator() + selectedVault);
    if (vaultDir.removeRecursively()) {
      vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
    } else {
      QMessageBox::critical(this, tr("Error"),
                            tr("Failed to delete vault directory."));
    }
  }
}
