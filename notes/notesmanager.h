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

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void filterEntries(const QString &filterText);
  void addNoteClicked();

private:
  Ui::NotesManager *ui;

  void updateStackWidget();
};

#endif // NOTESMANAGER_H
