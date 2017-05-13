#include "u32backend.hpp"

#include <QCursor>
#include <QtWin>
#include <windows.h>

QPixmap PlatformBackend::getCursor() {
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(cursorInfo);
    if (GetCursorInfo(&cursorInfo)) {
        if (cursorInfo.flags == CURSOR_SHOWING) {
            ICONINFO info; // It took me 5 hours to get to here
            if (GetIconInfo(cursorInfo.hCursor, &info)) {
                return QtWin::fromHBITMAP(info.hbmColor);
            } else
                return QPixmap();
        } else
            return QPixmap();
    } else
        return QPixmap();
}
