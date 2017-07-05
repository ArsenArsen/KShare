#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap screenshotutil::fullscreen(bool cursor) {
    QPixmap image;
    QPainter painter;
    QPoint smallestCoordinate = smallestScreenCoordinate();

// Hack for https://bugreports.qt.io/browse/QTBUG-58110
#ifdef Q_OS_LINUX
    static QStringList qVer = QString(qVersion()).split('.');
    if (qVer.at(0).toInt() == 5 && qVer.at(1).toInt() < 9) {
        image = window(0);
        painter.begin(&image);
    } else {
#endif
        int height = 0, width = 0;
        int ox = smallestCoordinate.x() * -1, oy = smallestCoordinate.y() * -1;
        for (QScreen *screen : QApplication::screens()) {
            QRect geo = screen->geometry();
            width = qMax(ox + geo.left() + geo.width(), width);
            height = qMax(oy + geo.top() + geo.height(), height); // qute abs
        }
        image = QPixmap(width, height);
        image.fill(Qt::transparent);
        width = 0;
        painter.begin(&image);
        painter.translate(ox, oy);

        for (QScreen *screen : QApplication::screens()) {
            QPixmap currentScreen = window(0, screen);
            painter.drawPixmap(screen->geometry().topLeft(), currentScreen);
            width += screen->size().width();
        }
#ifdef Q_OS_LINUX
    }
#endif

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

QPoint screenshotutil::smallestScreenCoordinate() {
    QPoint smallestCoordinate;
    for (QScreen *screen : QApplication::screens()) {
        smallestCoordinate.rx() = qMin(smallestCoordinate.x(), screen->geometry().left());
        smallestCoordinate.ry() = qMin(smallestCoordinate.y(), screen->geometry().top());
    }
    return smallestCoordinate;
}
