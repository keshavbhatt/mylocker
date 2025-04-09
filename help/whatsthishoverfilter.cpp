#include "whatsthishoverfilter.h"

WhatsThisHoverFilter::WhatsThisHoverFilter(QObject *parent) : QObject(parent) {}

void WhatsThisHoverFilter::installWhatsThisHoverFilter(QWidget *root, QObject *filter) {
    root->installEventFilter(filter);
    const auto children = root->findChildren<QWidget *>();
    for (QWidget *child : children) {
        child->installEventFilter(filter);
    }
}

bool WhatsThisHoverFilter::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::ToolTip) {
        QWidget *widget = qobject_cast<QWidget *>(watched);
        if (widget && !widget->whatsThis().isEmpty()) {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            QToolTip::showText(helpEvent->globalPos(), widget->whatsThis(), widget);
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}
