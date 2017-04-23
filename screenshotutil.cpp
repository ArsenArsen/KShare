#include "screenshotutil.hpp"

#include <QApplication>
#include <QClipboard>
#include <QPixmap>
#include <QScreen>

QPixmap *screenshotutil::fullscreen()
{
    return window(0);
}

QPixmap *screenshotutil::window(long wid)
{
    QScreen *w = QApplication::primaryScreen();
    QPixmap screen = w->grabWindow(wid);
    QPixmap *pm = new QPixmap(screen.size());
    screen.swap(*pm);
    return pm;
}

void screenshotutil::toClipboard(QString value)
{
    QApplication::clipboard()->setText(value);
}
