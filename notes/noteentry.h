#ifndef NOTEENTRY_H
#define NOTEENTRY_H

#include <QDateTime>
#include <QString>

struct NoteEntry {
  QString id;
  QString title;
  QString content;
  QDateTime createdAt;
  QDateTime updatedAt;

  static NoteEntry create(const QString &title, const QString &content);
};

#endif // NOTEENTRY_H
