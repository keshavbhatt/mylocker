#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

#include <login/autolockmanager.h>

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
  void openPasswordManager();
  void openSecureNotes();
  void onLockNowClicked();

private:
  Ui::Dashboard *ui;

private slots:
  void updateAutoLockLabel(int seconds);
};

#endif // DASHBOARD_H
