#include "macbackend.hpp"

#include <pwd.h>
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
