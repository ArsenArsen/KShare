#ifndef U32BACKEND_HPP
#define U32BACKEND_HPP

#include <QPixmap>
#include <windows.h>

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    DWORD pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // U32BACKEND_HPP
