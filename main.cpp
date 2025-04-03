#include "mainwindow.h"

#include <QApplication>
#include <setup/setupdialog.h>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QString masterHash =
      SettingsManager::instance().getValue("security/master_hash").toString();

  if (masterHash.isEmpty()) {
    SetupDialog setupDialog;
    if (setupDialog.exec() != QDialog::Accepted) {
      return 0;
    }
  }

  MainWindow w;
  w.show();

  return a.exec();
}
