#include "iconloader.h"

#include <QBuffer>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QRegularExpression>

namespace Utils {

const QStringList &IconLoader::getAvailableIcons() {
  if (cachedIconList.isEmpty()) {
    QDirIterator it(":/svg/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
      QString path = it.next();
      if (path.endsWith(".svg", Qt::CaseInsensitive)) {
        cachedIconList << path;
      }
    }
  }
  return cachedIconList;
}

QIcon IconLoader::loadColoredIcon(const QString &iconName, const QColor &color,
                                  QSize size) {
  const QString iconPath = QString(":/svg/%1.svg").arg(iconName);

  if (!svgCache.contains(iconName)) {
    QFile file(iconPath);
    if (!file.open(QIODevice::ReadOnly)) {
      qWarning() << "Failed to load icon:" << iconPath;
      return QIcon();
    }
    svgCache[iconName] = QString::fromUtf8(file.readAll());
  }

  QString svgContent = svgCache[iconName];

  svgContent.replace(QRegularExpression(R"(fill="#[0-9A-Fa-f]{6}")"),
                     QString("fill=\"%1\"").arg(color.name()));

  QPixmap pixmap = renderSvg(svgContent, size);
  return QIcon(pixmap);
}

QPixmap IconLoader::renderSvg(const QString &svgContent, const QSize &size) {
  QSvgRenderer renderer(svgContent.toUtf8());
  QPixmap pixmap(size);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  renderer.render(&painter);
  return pixmap;
}

} // namespace Utils
