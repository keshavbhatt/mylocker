#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include "newuserpage.h"

#include <QDialog>

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
