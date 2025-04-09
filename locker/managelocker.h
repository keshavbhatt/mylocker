#ifndef MANAGELOCKER_H
#define MANAGELOCKER_H

#include <QWidget>

#include <vault/vaultlistwidget.h>

namespace Ui {
class ManageLocker;
}

class ManageLocker : public QWidget {
  Q_OBJECT

public:
  explicit ManageLocker(const QString &lockerDataDirPath,
                        QWidget *parent = nullptr);
  ~ManageLocker();

private slots:
  void deleteSelectedVault();
  void createNewVault();

private:
  QString m_lockerDataDirPath;
  Ui::ManageLocker *ui;

  VaultListWidget *vaultListWidget = nullptr;
  void updateVaultActionButtons(const QString &vaultName);
};

#endif // MANAGELOCKER_H
