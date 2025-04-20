#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWidget>

#include <icons/iconloader.h>
#include <notes/storage/notesstorage.h>
#include <theme/palette.h>
#include <vault/vaultmanager.h>

namespace Ui {
class NotesManager;
}

class NotesManager : public QWidget {
  Q_OBJECT

public:
  explicit NotesManager(QWidget *parent = nullptr);
  ~NotesManager();

signals:
  void goToDashboard();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void filterEntries(const QString &filterText);
  void addNoteClicked();
  void noteError(const QString &error);
  void handleNoteUpdated(const NoteEntry &updatedEntry);
  void handleNoteDeleted(QUuid id);
  void noteAdded(NoteEntry entry);
  void notesLoaded(QVector<NoteEntry> entries);

private:
  Ui::NotesManager *ui;

  void updateStackWidget();
  void stressTest();
  void confirmAndDeleteNote(QUuid id);
  void addNoteCardToUi(const NoteEntry &entry);
};

#endif // NOTESMANAGER_H
