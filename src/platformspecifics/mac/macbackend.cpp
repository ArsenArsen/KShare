#include "macbackend.hpp"

#include <unistd.h>

std::tuple<QPoint, QPixmap> PlatformBackend::getCursor() {
#warning "TODO: Mac backend"
    return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
    // Not Monday: https://developer.apple.com/reference/appkit/nscursor/1527062-image
    // Some on how to do NSImage -> QPixmap: http://stackoverflow.com/a/2468961/3809164
    // This is gonna be easier than with Windows
}

pid_t PlatformBackend::pid() {
    return getpid();
}

bool PlatformBackend::filenameValid(QString name) {
    return !name.contains('/');
}
