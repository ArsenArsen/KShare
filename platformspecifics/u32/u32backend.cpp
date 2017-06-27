#include "u32backend.hpp"

#include <QCursor>
#include <QtWin>
#include <windows.h>

std::tuple<QPoint, QPixmap> PlatformBackend::getCursor() {
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(cursorInfo);
    if (GetCursorInfo(&cursorInfo)) {
        if (cursorInfo.flags == CURSOR_SHOWING) {
            ICONINFO info; // It took me 5 hours to get to here
            if (GetIconInfo(cursorInfo.hCursor, &info)) {
                return std::tuple<QPoint, QPixmap>(QPoint(info.xHotspot, info.yHotspot),
                                                   QtWin::fromHBITMAP(info.hbmColor, QtWin::HBitmapAlpha));
            } else
                return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
        } else
            return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
    } else
        return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
}

DWORD PlatformBackend::pid() {
    return GetCurrentProcessId();
}

WId PlatformBackend::getActiveWID() {
    return (WId)GetForegroundWindow();
}
