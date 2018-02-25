#include "u32backend.hpp"

#include <Lmcons.h>
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

QString illegal(QStringLiteral("<>:\"/\\|?*"));
QStringList illegalNames({ "CON",  "PRN",  "AUX",  "NUL",  "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7",
                           "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9" });

bool PlatformBackend::filenameValid(QString name) {
    int periods = 0;
    for (QChar c : name) {
        if (c == '.') periods++;
        if (illegal.contains(c)) return false;
        if (c < 32) return false;
    }
    if (periods == name.length()) return false;
    return !illegalNames.contains(name);
}

QString PlatformBackend::getCurrentUser() {
    WCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    QString userName;
    if (GetUserName(username, &username_len)) {
        userName = QString::fromWCharArray(username, username_len - 1);
    }
    delete[] username;
    return userName;
}
