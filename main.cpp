#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <setup/setupdialog.h>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QApplication::setApplicationName(APPLICATION_FULLNAME);
  QApplication::setDesktopFileName("com.ktechpit.mylocker");
  QApplication::setOrganizationDomain("com.ktechpit");
  QApplication::setOrganizationName("org.keshavnrj.ubuntu");
  QApplication::setApplicationVersion(VERSIONSTR);

  QSettings settings;
  QString lockerDataDirPath = settings.value("lockerDataDirPath").toString();

  if (lockerDataDirPath.isNull() || lockerDataDirPath.trimmed().isEmpty()) {
    SetupDialog setupDialog;
    if (setupDialog.exec() != QDialog::Accepted) {
      return 0;
    }
  }

  MainWindow w;
  w.show();

  return a.exec();
}
