#ifndef X11BACKEND_HPP
#define X11BACKEND_HPP

#include <QPixmap>

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    pid_t pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // X11BACKEND_HPP
