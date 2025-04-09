#ifndef NEWUSERPAGE_H
#define NEWUSERPAGE_H

#include <QEvent>
#include <QWidget>

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
