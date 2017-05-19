#ifndef SCREENSHOTUTIL_HPP
#define SCREENSHOTUTIL_HPP

#include <QWidget>

namespace screenshotutil {
QPixmap *fullscreen(bool cursor = true);
QPixmap *fullscreenArea(bool cursor = true, qreal x = 0, qreal y = 0, qreal w = -1, qreal h = -1);
QPixmap *window(WId wid);
void toClipboard(QString value);
}

#endif // SCREENSHOTUTIL_HPP
