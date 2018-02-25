#include "x11backend.hpp"

#include <QPixmap>
#include <QX11Info>
#include <pwd.h>
#include <unistd.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_util.h>
#include <xcb/xfixes.h>

std::tuple<QPoint, QPixmap> PlatformBackend::getCursor() {
    xcb_connection_t *connection = QX11Info::connection();
    xcb_xfixes_get_cursor_image_cookie_t cursorCookie = xcb_xfixes_get_cursor_image_unchecked(connection);
    QScopedPointer<xcb_xfixes_get_cursor_image_reply_t> cursorReply(xcb_xfixes_get_cursor_image_reply(connection, cursorCookie, NULL));
    if (cursorReply.isNull()) {
        return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
    }

    quint32 *pixels = xcb_xfixes_get_cursor_image_cursor_image(cursorReply.data());
    if (!pixels) {
        return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
    }
    return std::tuple<QPoint, QPixmap>(QPoint(cursorReply->xhot, cursorReply->yhot),
                                       QPixmap::fromImage(QImage((quint8 *)pixels, cursorReply->width, cursorReply->height,
                                                                 QImage::Format_ARGB32_Premultiplied)));
}

pid_t PlatformBackend::pid() {
    return getpid();
}

WId PlatformBackend::getActiveWID() {
    xcb_connection_t *connection = QX11Info::connection();
    xcb_get_input_focus_reply_t *focusReply;
    xcb_query_tree_cookie_t treeCookie;
    xcb_query_tree_reply_t *treeReply;

    focusReply = xcb_get_input_focus_reply(connection, xcb_get_input_focus(connection), nullptr);
    xcb_window_t window = focusReply->focus;
    while (1) {
        treeCookie = xcb_query_tree(connection, window);
        treeReply = xcb_query_tree_reply(connection, treeCookie, nullptr);
        if (!treeReply) {
            window = 0;
            break;
        }
        if (window == treeReply->root || treeReply->parent == treeReply->root) {
            break;
        } else {
            window = treeReply->parent;
        }
        delete treeReply;
    }
    delete treeReply;
    return window;
}

bool PlatformBackend::filenameValid(QString name) {
    return !name.contains('/');
}

QString PlatformBackend::getCurrentUser() {
    auto pwent = getpwuid(getuid());
    if (!pwent) {
        if (qEnvironmentVariableIsSet("USER"))
            return QString::fromLocal8Bit(qgetenv("USER"));
        else
            return QString();
    }
    QString ret = QString::fromLocal8Bit(pwent->pw_name);
    endpwent();
    return ret;
}
