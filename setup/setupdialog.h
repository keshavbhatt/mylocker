#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog {
  Q_OBJECT

public:
  explicit SetupDialog(QWidget *parent = nullptr);
  ~SetupDialog();

private:
  Ui::SetupDialog *ui;
  void setMasterPassword();
};

#endif // SETUPDIALOG_H
