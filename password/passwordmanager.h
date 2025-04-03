#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include "passwordworker.h"

#include <QFrame>
#include <QUuid>
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

private slots:
  void addPasswordClicked();
  void passwordsLoaded(QVector<PasswordEntry> entries);
  void passwordAdded(PasswordEntry entry);
  void handlePasswordDeleted(QUuid id);
  void passwordError(const QString &error);
  void updateStackWidget();

private:
  Ui::PasswordManager *ui;
  QFrame *createPasswordCard(const PasswordEntry &entry);
  void addPasswordCardToUi(const PasswordEntry &entry);
  void confirmAndDeletePassword(QUuid id);
};

#endif // PASSWORDMANAGER_H
