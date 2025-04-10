#ifndef LAYOUT_H
#define LAYOUT_H

#include <QLayout>
#include <QWidget>

namespace Utils::Layout {

inline void clearLayout(QLayout *layout) {
  if (!layout)
    return;

  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != nullptr) {
    if (QWidget *widget = item->widget()) {
      widget->deleteLater();
    } else if (QLayout *childLayout = item->layout()) {
      clearLayout(childLayout);
    }
    delete item;
  }
}
} // namespace Utils::Layout

#endif // LAYOUT_H
