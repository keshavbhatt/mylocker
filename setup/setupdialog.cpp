#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SetupDialog) {
  ui->setupUi(this);

  setWindowTitle(APPLICATION_FULLNAME);
  ui->welcomeLabel->setText(QString("Welcome to ") + APPLICATION_FULLNAME);

  newUserPage = new NewUserPage(this);
  connect(newUserPage, &NewUserPage::backToWelcomePageRequested, this,
          &SetupDialog::showWelcomePage);
  connect(newUserPage, &NewUserPage::setupComplete, this, &SetupDialog::accept);

  ui->stackedWidget->addWidget(newUserPage);

  ui->existingDir->setIcon(Utils::IconLoader::loadColoredIcon(
      "folder-keyhole-fill", Palette::iconDefault()));
  connect(ui->existingDir, &QPushButton::clicked, this,
          &SetupDialog::handleExistingDataDirectorySelection);

  ui->newUser->setIcon(Utils::IconLoader::loadColoredIcon(
      "shield-cross-fill", Palette::iconDefault()));
  connect(ui->newUser, &QPushButton::clicked, this,
          [=]() { ui->stackedWidget->slideInWgt(newUserPage); });

  ui->stackedWidget->slideInWgt(ui->page_welcome);
}

void SetupDialog::showWelcomePage() {
  ui->stackedWidget->slideInWgt(ui->page_welcome);
}

void SetupDialog::handleExistingDataDirectorySelection() {
  QString existingDataDirectoryPath = QFileDialog::getExistingDirectory(
      this, tr("Select Application Data Directory"), QDir::homePath(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (existingDataDirectoryPath.isEmpty()) {
    return;
  }

  QDir selectedDir(existingDataDirectoryPath);
  QFile settingsFile(selectedDir.filePath(".settings.ini"));
  if (!settingsFile.exists()) {
    QMessageBox::warning(
        this, "Invalid Directory",
        "The selected directory does not appear to be a valid application "
        "data directory (missing .settings.ini).");
    return;
  }

  VaultSelectionDialog dialog(existingDataDirectoryPath, this);
  if (dialog.exec() == QDialog::Accepted) {
    QString vaultName = dialog.selectedVaultName();
    Vault newVault(vaultName);

    if (!VaultManager::instance().openVault(newVault,
                                            existingDataDirectoryPath)) {
      QMessageBox::critical(this, "Error", "Failed to open vault.");
      return;
    }

    QSettings settings;
    settings.setValue("lockerDataDirPath", existingDataDirectoryPath);
    accept();
  }
}

SetupDialog::~SetupDialog() { delete ui; }
