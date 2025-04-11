#include "autolockmanager.h"
#include <QApplication>
#include <QEvent>

AutoLockManager::AutoLockManager(QObject *parent)
    : QObject(parent), remainingTime(0) {
  int autoLockInterval =
      m_settings.value("security/auto_lock_time", 30).toInt();

  autoLockTimer = new QTimer(this);
  autoLockTimer->setInterval(autoLockInterval * 1000);
  autoLockTimer->setSingleShot(true);
  connect(autoLockTimer, &QTimer::timeout, this, &AutoLockManager::timeout);

  countdownTimer = new QTimer(this);
  connect(countdownTimer, &QTimer::timeout, this,
          &AutoLockManager::updateRemainingTime);

  qApp->installEventFilter(this);
}

void AutoLockManager::start() {
  remainingTime = autoLockTimer->interval() / 1000;
  autoLockTimer->start();
  countdownTimer->start(1000);
}

void AutoLockManager::reset() {
  remainingTime = autoLockTimer->interval() / 1000;
  autoLockTimer->start();
  countdownTimer->start(1000);
  emit remainingTimeUpdated(remainingTime);
}

void AutoLockManager::setLockInterval(int seconds) {
  autoLockTimer->setInterval(seconds * 1000);
  reset();
}

void AutoLockManager::updateRemainingTime() {
  if (remainingTime > 0) {
    remainingTime--;
    emit remainingTimeUpdated(remainingTime);
  } else {
    countdownTimer->stop();
  }
}

bool AutoLockManager::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseMove || event->type() == QEvent::KeyPress ||
      event->type() == QEvent::MouseButtonPress ||
      event->type() == QEvent::Wheel) {
    reset();
  }
  return QObject::eventFilter(obj, event);
}
