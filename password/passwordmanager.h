#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include "passwordworker.h"

#include <QComboBox>
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
