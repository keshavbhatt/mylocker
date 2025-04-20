#ifndef ADDNOTEDIALOG_H
#define ADDNOTEDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "noteentry.h"
#include <categories/categorymanager.h>

class AddNoteDialog : public QDialog {
  Q_OBJECT

public:
  explicit AddNoteDialog(QWidget *parent = nullptr);
  explicit AddNoteDialog(const NoteEntry &entry, QWidget *parent = nullptr);

  NoteEntry getNoteEntry(const QUuid &existingId = QUuid()) const;

private:
  QLineEdit *titleInput;
  QTextEdit *contentInput;
  QComboBox *categoryCombo;

  QPushButton *saveBtn;
  QPushButton *cancelBtn;
};

#endif // ADDNOTEDIALOG_H
