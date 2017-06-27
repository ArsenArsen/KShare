#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap *screenshotutil::fullscreen(bool cursor) {
#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        QPixmap *noCursor = window(0);
        QScopedPointer<QPixmap> p(noCursor);
        QPixmap *withCursor = new QPixmap(*noCursor);
        QPainter painter(withCursor);
        auto cursorData = PlatformBackend::inst().getCursor();
        painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData), std::get<1>(cursorData));
        painter.end();
        return withCursor;
    }
#endif
    return window(0);
}

QPixmap *screenshotutil::window(WId wid) {
    QScreen *w = QApplication::primaryScreen();
    QPixmap screen = w->grabWindow(wid);
    QPixmap *pm = new QPixmap(screen.size());
    screen.swap(*pm);
    return pm;
}

void screenshotutil::toClipboard(QString value) {
    QApplication::clipboard()->setText(value);
}

QPixmap *screenshotutil::fullscreenArea(bool cursor, qreal x, qreal y, qreal w, qreal h) {
    auto scr = QApplication::primaryScreen();
    QRectF area(x, y, w < 0 ? scr->size().width() : w, h < 0 ? scr->size().height() : h);
#ifdef PLATFORM_CAPABILITY_CURSOR
    if (cursor) {
        QPointF point = QCursor::pos(scr);
        if (area.contains(point)) {
            QPixmap noCursor = scr->grabWindow(0, area.x(), area.y(), area.width(), area.height());
            QPixmap *withCursor = new QPixmap(noCursor);
            QPainter painter(withCursor);
            auto cursorData = PlatformBackend::inst().getCursor();
            painter.drawPixmap(QCursor::pos() - std::get<0>(cursorData) - area.topLeft().toPoint(), std::get<1>(cursorData));
            painter.end();
            return withCursor;
        }
    }
#endif
    return new QPixmap(scr->grabWindow(0, area.x(), area.y(), area.width(), area.height()));
}
