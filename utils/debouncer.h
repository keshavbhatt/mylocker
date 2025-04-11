#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <QObject>
#include <QTimer>
#include <functional>

namespace Utils {
class Debouncer : public QObject {
  Q_OBJECT

public:
  Debouncer(int intervalMs = 200, QObject *parent = nullptr) : QObject(parent) {
    m_timer.setInterval(intervalMs);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]() {
      if (m_callback)
        m_callback();
    });
  }

  void debounce(std::function<void()> callback) {
    m_callback = std::move(callback);
    m_timer.start();
  }

  void setInterval(int ms) { m_timer.setInterval(ms); }

private:
  QTimer m_timer;
  std::function<void()> m_callback;
};
}      // namespace Utils
#endif // DEBOUNCER_H
