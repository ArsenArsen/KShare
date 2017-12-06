#ifndef UTILS_HPP
#define UTILS_HPP

#include <QApplication>
#include <QPixmap>
#include <QWidget>
#include <QScreen>

namespace utils {
    QColor invertColor(QColor color);
    QPixmap extend(QPixmap pixmap, int extraSize = 25, QColor hl = Qt::transparent);
    QPixmap fullscreen(bool cursor = true);
    QPixmap fullscreenArea(bool cursor = true, qreal x = 0, qreal y = 0, qreal w = -1, qreal h = -1);
    QPixmap window(WId wid, QScreen *w = QApplication::primaryScreen());
    void toClipboard(QString value);
    QPoint smallestScreenCoordinate();
    QPixmap
    renderText(QString toRender, int padding = 5, QColor background = Qt::transparent, QColor pen = Qt::white, QFont font = QFont());
}

#endif // UTILS_HPP
