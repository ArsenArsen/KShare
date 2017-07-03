#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap screenshotutil::fullscreen(bool cursor) {
    int height = 0, width = 0;
    for (QScreen *screen : QApplication::screens()) {
        QRect geo = screen->geometry();
        width = qMax(geo.right() + geo.width(), width);
        height = qMax(geo.bottom() + geo.height(), height);
    }
    QPixmap image(width, height);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    width = 0;
    for (QScreen *screen : QApplication::screens()) {
        QPixmap currentScreen = window(0, screen);
        // Hack for https://bugreports.qt.io/browse/QTBUG-58110
        QStringList qVer = QString(qVersion()).split('.');
        if (qVer.at(0).toInt() == 5 && qVer.at(1).toInt() < 9) currentScreen = currentScreen.copy(screen->geometry());
        painter.drawPixmap(screen->geometry().topLeft(), currentScreen);
        width += screen->size().width();
    }
#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        auto cursorData = PlatformBackend::inst().getCursor();
        painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData), std::get<1>(cursorData));
    }
#endif
    painter.end();
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
