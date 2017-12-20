#ifndef U32BACKEND_HPP
#define U32BACKEND_HPP

#include <QPixmap>
#include <windows.h>

#define PLATFORM_CAPABILITY_PID
#define PLATFORM_CAPABILITY_ACTIVEWINDOW
#define PLATFORM_CAPABILITY_CURSOR
#define PLATFORM_CAPABILITY_CURRENT_USER

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    DWORD pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
    WId getActiveWID();
    bool filenameValid(QString name);
    QString getCurrentUser();
};

#endif // U32BACKEND_HPP
