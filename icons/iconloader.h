#ifndef ICONLOADER_H
#define ICONLOADER_H

#include <QIcon>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QSvgRenderer>

namespace Utils {

class IconLoader {
public:
  /**
   * Load an icon by name (without path or extension), apply color and return
   * QIcon
   */
  static QIcon loadColoredIcon(const QString &iconName,
                               const QColor &color = Qt::black,
                               QSize size = QSize(64, 64));

  /**
   * Get available Icon names from icon resource(svg) folder
   */
  static const QStringList &getAvailableIcons();

private:
  /**
   * Generate pixmap from raw SVG content and color
   */
  static QPixmap renderSvg(const QString &svgContent, const QSize &size);

  /**
   * Cacheed available icons for performance
   */
  inline static QStringList cachedIconList;

  /**
   * Cache for performance
   */
  static inline QMap<QString, QString> svgCache; // iconName -> SVG content
};
} // namespace Utils
#endif // ICONLOADER_H
