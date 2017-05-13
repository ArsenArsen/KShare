#include "macbackend.hpp"

QPixmap PlatformBackend::getCursor() {
#warning "TODO: Mac backend"
    return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
    // Monday: https://developer.apple.com/reference/appkit/nscursor/1527062-image
    // Some on how to do NSImage -> QPixmap: http://stackoverflow.com/a/2468961/3809164
    // This is gonna be easier than with Windows
}
