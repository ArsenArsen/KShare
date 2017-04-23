#ifndef SCREENSHOTUTIL_HPP
#define SCREENSHOTUTIL_HPP

#include <QPixmap>

namespace screenshotutil
{
QPixmap *fullscreen();
QPixmap *window(long wid);
void toClipboard(QString value);
}

#endif // SCREENSHOTUTIL_HPP
