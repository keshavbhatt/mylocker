#ifndef WHATSTHISHOVERFILTER_H
#define WHATSTHISHOVERFILTER_H

#include <QEvent>
#include <QHelpEvent>
#include <QObject>
#include <QToolTip>
#include <QWidget>

class WhatsThisHoverFilter : public QObject {
public:
  explicit WhatsThisHoverFilter(QObject *parent = nullptr);

  static void installWhatsThisHoverFilter(QWidget *root, QObject *filter);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // WHATSTHISHOVERFILTER_H
