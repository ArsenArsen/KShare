#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap *screenshotutil::fullscreen(bool cursor) {
    int height = 0, width = 0;
    for (QScreen *screen : QApplication::screens()) {
        width += screen->size().width();
        int h = screen->size().height();
        height = h > height ? h : height;
    }
    QPixmap *image = new QPixmap(width, height);
    image->fill(Qt::transparent);
    QPainter painter(image);
    width = 0;
    for (QScreen *screen : QApplication::screens()) {

        QPixmap *currentScreen = window(0, screen);
        painter.drawPixmap(width, 0, currentScreen->copy());
        delete currentScreen;
        width += screen->size().width();
    }
    painter.end();
#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        QPixmap *noCursor = image;
        QScopedPointer<QPixmap> p(noCursor);
        QPixmap *withCursor = new QPixmap(*noCursor);
        QPainter painter(withCursor);
        auto cursorData = PlatformBackend::inst().getCursor();
        painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData), std::get<1>(cursorData));
        painter.end();
        return withCursor;
    }
#endif
    return image;
}

QPixmap *screenshotutil::window(WId wid, QScreen *w) {
    QPixmap screen = w->grabWindow(wid);
    QPixmap *pm = new QPixmap(screen.size());
    screen.swap(*pm);
    return pm;
}

void screenshotutil::toClipboard(QString value) {
    QApplication::clipboard()->setText(value);
}

QPixmap *screenshotutil::fullscreenArea(bool cursor, qreal x, qreal y, qreal w, qreal h) {
    QPixmap *cropped = new QPixmap;
    QPixmap *scr = fullscreen(cursor);
    scr->copy(x, y, w, h).swap(*cropped);
    delete scr;
    return cropped;
}
