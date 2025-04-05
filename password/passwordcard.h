#ifndef PASSWORDCARD_H
#define PASSWORDCARD_H

#include "passwordworker.h"

#include <QFrame>
#include <QLabel>
#include <QToolButton>
#include <QUuid>

class PasswordCard : public QFrame {
  Q_OBJECT
public:
  explicit PasswordCard(const PasswordEntry &entry, QWidget *parent = nullptr);

  void setEntry(const PasswordEntry &entry);
  void refresh();

signals:
  void deleteRequested(const QUuid &id);
  void editRequested(const PasswordEntry &entry);
  void duplicateRequested(const PasswordEntry &entry);
  void entryEdited(const PasswordEntry &updatedEntry);

private:
  void setupUI();

  PasswordEntry m_entry;
  QLabel *m_titleLabel = nullptr;
  QLabel *m_userLabel = nullptr;
  QLabel *m_passwordLabel = nullptr;
  QToolButton *m_togglePassBtn = nullptr;

  QWidget *m_detailsWidget = nullptr;
  void showFullDetailsDialog();
  QToolButton *createCopyToolButton();
  void showEditDialog();
};

#endif // PASSWORDCARD_H
