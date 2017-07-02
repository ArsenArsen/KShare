#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap screenshotutil::fullscreen(bool cursor) {
    int height = 0, width = 0;
    for (QScreen *screen : QApplication::screens()) {
        width += screen->size().width();
        int h = screen->size().height();
        height = h > height ? h : height;
    }
    QPixmap image(width, height);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    width = 0;
    for (QScreen *screen : QApplication::screens()) {
        QPixmap currentScreen = window(0, screen);
        painter.drawPixmap(width, 0, currentScreen);
        width += screen->size().width();
    }
#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        auto cursorData = PlatformBackend::inst().getCursor();
        painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData), std::get<1>(cursorData));
    }
    painter.end();
#endif
    return image;
}

QPixmap screenshotutil::window(WId wid, QScreen *w) {
    return w->grabWindow(wid);
}

void screenshotutil::toClipboard(QString value) {
    QApplication::clipboard()->setText(value);
}

QPixmap screenshotutil::fullscreenArea(bool cursor, qreal x, qreal y, qreal w, qreal h) {
    return fullscreen(cursor).copy(x, y, w, h);
}
