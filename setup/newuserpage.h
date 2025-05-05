#ifndef NEWUSERPAGE_H
#define NEWUSERPAGE_H

#include <QAction>
#include <QEvent>
#include <QFileDialog>
#include <QHelpEvent>
#include <QMessageBox>
#include <QSettings>
#include <QToolTip>
#include <QWidget>

#include <help/whatsthishoverfilter.h>
#include <icons/iconloader.h>
#include <security-manager/securitymanager.h>
#include <theme/palette.h>
#include <vault/vault.h>
#include <vault/vaultmanager.h>

namespace Ui {
class NewUserPage;
}

class NewUserPage : public QWidget {
  Q_OBJECT

public:
  explicit NewUserPage(QWidget *parent = nullptr);
  ~NewUserPage();

signals:
  void backToWelcomePageRequested();
  void setupComplete();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void selectNewDataDirectoryActionClicked();

private:
  Ui::NewUserPage *ui;
  void validateAndContinueSetup();
};

#endif // NEWUSERPAGE_H
