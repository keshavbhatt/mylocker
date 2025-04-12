#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include "addpassworddialog.h"
#include "passwordcard.h"
#include "passwordworker.h"
#include <categories/categorymanager.h>
#include <password/storage/passwordstorage.h>
#include <vault/vaultmanager.h>

#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDebug>
#include <QFrame>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTextEdit>
#include <QUuid>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class PasswordManager;
}

class PasswordManager : public QWidget {
  Q_OBJECT

public:
  explicit PasswordManager(QWidget *parent = nullptr);
  ~PasswordManager();

signals:
  void goToDashboard();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void addPasswordClicked();
  void passwordsLoaded(QVector<PasswordEntry> entries);
  void passwordAdded(PasswordEntry entry);
  void handlePasswordDeleted(QUuid id);
  void passwordError(const QString &error);
  void handlePasswordUpdated(const PasswordEntry &updatedEntry);
  void filterEntries(const QString &filterText);

private:
  Ui::PasswordManager *ui;

  void updateStackWidget();
  void addPasswordCardToUi(const PasswordEntry &entry);
  void confirmAndDeletePassword(QUuid id);
  void stressTest();
};

#endif // PASSWORDMANAGER_H
