#ifndef X11BACKEND_HPP
#define X11BACKEND_HPP

#include <QPixmap>

#define PLATFORM_CAPABILITY_PID
#define PLATFORM_CAPABILITY_ACTIVEWINDOW
#define PLATFORM_CAPABILITY_CURSOR
#define PLATFORM_CAPABILITY_CURRENT_USER

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    pid_t pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
    WId getActiveWID();
    bool filenameValid(QString name);
    QString getCurrentUser();
};

#endif // X11BACKEND_HPP
