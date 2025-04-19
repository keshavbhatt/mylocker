#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QDateTime>
#include <QUuid>
#include <QVector>

struct PasswordEntry {
  // basic fields
  QUuid id;
  QString title;
  QString username;
  QString password;
  QDateTime createdAt;
  QDateTime updatedAt;

  // extended fields
  QString url;
  QString notes;
  QString category;

  PasswordEntry()
      : id(QUuid::createUuid()), createdAt(QDateTime::currentDateTime()) {}
};

#endif // PASSWORDENTRY_H
