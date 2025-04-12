#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDebug>
#include <QWidget>

#include <icons/iconloader.h>
#include <login/autolockmanager.h>
#include <notes/notesmanager.h>
#include <password/passwordmanager.h>
#include <theme/palette.h>
#include <vault/vaultmanager.h>

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget {
  Q_OBJECT

public:
  explicit Dashboard(QWidget *parent = nullptr);
  ~Dashboard();
  void setAutoLockManager(class AutoLockManager *manager);

signals:
  void lockApplicationRequested();

protected slots:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  Ui::Dashboard *ui;
  PasswordManager *passwordManager = nullptr;
  NotesManager *notesManager = nullptr;

private slots:
  void updateAutoLockLabel(int seconds);
  void showPasswordManager();
  void showNotesManager();
};

#endif // DASHBOARD_H
