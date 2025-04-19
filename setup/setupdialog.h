#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QAction>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "newuserpage.h"
#include <icons/iconloader.h>
#include <security-manager/securitymanager.h>
#include <theme/palette.h>
#include <vault/vault.h>
#include <vault/vaultmanager.h>
#include <vault/vaultselectiondialog.h>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog {
  Q_OBJECT

public:
  explicit SetupDialog(QWidget *parent = nullptr);
  ~SetupDialog();

private slots:
  void showWelcomePage();
  void handleExistingDataDirectorySelection();

private:
  Ui::SetupDialog *ui;

  NewUserPage *newUserPage = nullptr;
};

#endif // SETUPDIALOG_H
