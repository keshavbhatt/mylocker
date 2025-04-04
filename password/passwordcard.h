#ifndef PASSWORDCARD_H
#define PASSWORDCARD_H

#include "passwordworker.h"

#include <QFrame>
#include <QLabel>
#include <QUuid>

class PasswordCard : public QFrame {
  Q_OBJECT
public:
  explicit PasswordCard(const PasswordEntry &entry, QWidget *parent = nullptr);

signals:
  void deleteRequested(const QUuid &id);
  void editRequested(const PasswordEntry &entry);

private:
  void setupUI();

  PasswordEntry m_entry;
  QLabel *m_passwordLabel = nullptr;
  QWidget *m_detailsWidget = nullptr;
  void showFullDetailsDialog();
};

#endif // PASSWORDCARD_H
