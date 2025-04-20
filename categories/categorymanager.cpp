#include "categorymanager.h"

CategoryManager &CategoryManager::instance() {
  static CategoryManager instance;
  return instance;
}

CategoryManager::CategoryManager() { loadCategories(); }

QString CategoryManager::defaultCategory() const { return "Other"; }

void CategoryManager::loadCategories() {
  categories = presetCategories;

  QStringList savedCategories =
      SettingsManager::instance().getValue("customCategories").toStringList();

  for (const QString &cat : qAsConst(savedCategories)) {
    if (!categories.contains(cat)) {
      categories.append(cat);
    }
  }

  std::sort(categories.begin(), categories.end(),
            [](const QString &a, const QString &b) {
              return a.compare(b, Qt::CaseInsensitive) < 0;
            });
}

void CategoryManager::saveCategories() const {
  QStringList customCategories;

  for (const QString &cat : categories) {
    if (!presetCategories.contains(cat)) {
      customCategories.append(cat);
    }
  }

  SettingsManager::instance().setValue("customCategories", customCategories);
}

QVector<QString> CategoryManager::getCategories() const { return categories; }

void CategoryManager::addCategory(const QString &category) {
  if (!categories.contains(category)) {
    categories.append(category);
    saveCategories();
  }
}

bool CategoryManager::removeCategory(const QString &category) {
  if (!presetCategories.contains(category) && categories.removeOne(category)) {
    saveCategories();
    return true;
  }
  return false;
}
