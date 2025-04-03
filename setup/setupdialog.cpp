#include "setupdialog.h"
#include "ui_setupdialog.h"

#include <QMessageBox>

#include <security-manager/securitymanager.h>

SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SetupDialog) {
  ui->setupUi(this);

  setWindowTitle(APPLICATION_FULLNAME);
  ui->welcomeLabel->setText(QString("Welcome to ") + APPLICATION_FULLNAME);

  ui->masterPasswordLineEdit->setFocus();

  connect(ui->setMasterPasswordPushButton, &QPushButton::clicked, this,
          &SetupDialog::setMasterPassword);
}

SetupDialog::~SetupDialog() { delete ui; }

void SetupDialog::setMasterPassword() {
  QString password = ui->masterPasswordLineEdit->text();
  QString confirmPassword = ui->confirmMasterPasswordLineEdit->text();

  if (password.isEmpty() || password.length() < 6) {
    QMessageBox::warning(this, "Error",
                         "Password must be at least 6 characters.");
    return;
  }
  if (password != confirmPassword) {
    QMessageBox::warning(this, "Error", "Passwords do not match.");
    return;
  }

  SecurityManager::setMasterPassword(password);
  accept();
}
