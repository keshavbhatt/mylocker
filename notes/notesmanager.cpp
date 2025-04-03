#include "notesmanager.h"
#include "ui_notesmanager.h"

NotesManager::NotesManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::NotesManager) {
  ui->setupUi(this);

  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &NotesManager::goToDashboard);
}

NotesManager::~NotesManager() { delete ui; }
