#ifndef ADDPASSWORDDIALOG_H
#define ADDPASSWORDDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>

#include "passwordentry.h"
#include "passwordgenerator.h"
#include "passwordgeneratordialog.h"
#include <categories/categorymanager.h>
#include <passwordlineedit.h>

class AddPasswordDialog : public QDialog {
  Q_OBJECT

public:
  explicit AddPasswordDialog(QWidget *parent = nullptr);
  explicit AddPasswordDialog(const PasswordEntry &entry,
                             QWidget *parent = nullptr);

  PasswordEntry getPasswordEntry(const QUuid &existingId = NULL) const;

private:
  QLineEdit *siteInput;
  QLineEdit *usernameInput;
  PasswordLineEdit *passwordInput;
  QLineEdit *urlInput;
  QTextEdit *notesInput;
  QComboBox *categoryCombo;
  QGroupBox *extendedGroup;

  QPushButton *saveBtn;
  QPushButton *cancelBtn;
  QToolButton *createPasswordGeneratorButton(QLineEdit *targetInput,
                                             QWidget *parent);
};

#endif // ADDPASSWORDDIALOG_H
