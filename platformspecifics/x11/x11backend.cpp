#include "x11backend.hpp"

#include <QPixmap>
#include <QX11Info>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_util.h>
#include <xcb/xfixes.h>

QPixmap PlatformBackend::getCursor() {
    xcb_connection_t *connection = QX11Info::connection();
    xcb_xfixes_get_cursor_image_cookie_t cursorCookie = xcb_xfixes_get_cursor_image_unchecked(connection);
    QScopedPointer<xcb_xfixes_get_cursor_image_reply_t> cursorReply(xcb_xfixes_get_cursor_image_reply(connection, cursorCookie, NULL));
    if (cursorReply.isNull()) {
        return QPixmap();
    }

    quint32 *pixels = xcb_xfixes_get_cursor_image_cursor_image(cursorReply.data());
    if (!pixels) {
        return QPixmap();
    }
    return QPixmap::fromImage(QImage((quint8 *)pixels, cursorReply->width, cursorReply->height, QImage::Format_ARGB32_Premultiplied));
}
