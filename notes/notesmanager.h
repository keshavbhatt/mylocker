#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QWidget>

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

private:
  Ui::NotesManager *ui;
};

#endif // NOTESMANAGER_H
