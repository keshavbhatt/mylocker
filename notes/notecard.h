#ifndef NOTECARD_H
#define NOTECARD_H

#include "addnotedialog.h"
#include "noteentry.h"
#include "utils/layout.h"
#include <settings/settingsmanager.h>

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

class NoteCard : public QFrame {
  Q_OBJECT

public:
  explicit NoteCard(const NoteEntry &note, QWidget *parent = nullptr);

  NoteEntry entry() const;
  void setEntry(const NoteEntry &note);
  void refresh();

signals:
  void entryEdited(const NoteEntry &note);
  void entryDuplicated(const NoteEntry &note);
  void deleteRequested(const QString &id);

private:
  void setupUI();
  void showEditDialog();
  void showDuplicateDialog();
  void showFullDetailsDialog();
  void copyToClipboardSecurely(const QString &text);

  NoteEntry m_note;
  QLabel *m_titleLabel;
  QLabel *m_notePreviewLabel;
  QLabel *m_categoryLabel;
};

#endif // NOTECARD_H
