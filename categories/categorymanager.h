#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <algorithm>
#include <settings/settingsmanager.h>

#include <QMap>
#include <QString>
#include <QVector>

class CategoryManager {
public:
  static CategoryManager &instance();

  QVector<QString> getCategories() const;
  void addCategory(const QString &category);
  bool removeCategory(const QString &category);

  QString defaultCategory() const;

private:
  CategoryManager();
  void loadCategories();
  void saveCategories() const;

  QVector<QString> categories;
  const QVector<QString> presetCategories = {
      "Social",     "Banking",        "Work",          "Email",
      "Shopping",   "Streaming",      "Utilities",     "Gaming",
      "Travel",     "Cloud Services", "Education",     "Healthcare",
      "Government", "Development",    "Crypto",        "Insurance",
      "Finance",    "Devices",        "Entertainment", "Other"};
};

#endif // CATEGORYMANAGER_H
