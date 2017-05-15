#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <platformbackend.hpp>

QPixmap *screenshotutil::fullscreen(bool cursor) {
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
    return window(0);
}

QPixmap *screenshotutil::window(long wid) {
    QScreen *w = QApplication::primaryScreen();
    QPixmap screen = w->grabWindow(wid);
    QPixmap *pm = new QPixmap(screen.size());
    screen.swap(*pm);
    return pm;
}

void screenshotutil::toClipboard(QString value) {
    QApplication::clipboard()->setText(value);
}
