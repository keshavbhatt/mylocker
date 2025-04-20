#ifndef NOTEENTRY_H
#define NOTEENTRY_H

#include <QDateTime>
#include <QString>
#include <QUuid>

struct NoteEntry {
  QUuid id;
  QString title;
  QString content;
  QDateTime createdAt;
  QDateTime updatedAt;

  // extended fields
  QString category;

  NoteEntry()
      : id(QUuid::createUuid()), createdAt(QDateTime::currentDateTime()) {}
};

#endif // NOTEENTRY_H
