#ifndef PASSWORDCARD_H
#define PASSWORDCARD_H

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "addpassworddialog.h"
#include "passwordentry.h"
#include "utils/layout.h"
#include <settings/settingsmanager.h>

class PasswordCard : public QFrame {
  Q_OBJECT
public:
  explicit PasswordCard(const PasswordEntry &entry, QWidget *parent = nullptr);

  PasswordEntry entry() const;
  void setEntry(const PasswordEntry &entry);
  void refresh();

signals:
  void deleteRequested(const QUuid &id);
  void editRequested(const PasswordEntry &entry);
  void entryDuplicated(const PasswordEntry &newEntry);
  void entryEdited(const PasswordEntry &updatedEntry);

private slots:
  void showEditDialog();
  void showDuplicateDialog();

private:
  void setupUI();

  PasswordEntry m_entry; // TODO: do not hold entry just keep the id, and load
                         // entry on demand to reduce memory usages
  QLabel *m_titleLabel = nullptr;
  QLabel *m_userLabel = nullptr;
  QLabel *m_categoryLabel = nullptr;
  QLabel *m_passwordLabel = nullptr;
  QToolButton *m_togglePassBtn = nullptr;
  QWidget *m_detailsWidget = nullptr;
  QTimer *m_hidePasswordTimer = nullptr;

  void showFullDetailsDialog();
  void copyToClipboardSecurely(const QString &text);
  QToolButton *createCopyToolButton();
  QToolButton *createOpenUrlButton();
};

#endif // PASSWORDCARD_H
