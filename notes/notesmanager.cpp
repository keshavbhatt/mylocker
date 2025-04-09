#include "notesmanager.h"
#include "ui_notesmanager.h"

#include <QDebug>

NotesManager::NotesManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::NotesManager) {
  ui->setupUi(this);

  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &NotesManager::goToDashboard);
}

NotesManager::~NotesManager() {
  qDebug() << "Deleted NotesManager";
  delete ui;
}
