#ifndef AUTOLOCKMANAGER_H
#define AUTOLOCKMANAGER_H

#include <QObject>
#include <QTimer>

class AutoLockManager : public QObject {
  Q_OBJECT

public:
  explicit AutoLockManager(QObject *parent = nullptr);
  void start();
  void reset();
  void setLockInterval(int seconds);

signals:
  void timeout();
  void remainingTimeUpdated(int seconds);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private:
  QTimer *autoLockTimer;
  QTimer *countdownTimer;
  int remainingTime;

private slots:
  void updateRemainingTime();
};

#endif // AUTOLOCKMANAGER_H
