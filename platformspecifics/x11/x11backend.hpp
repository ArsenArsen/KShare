#ifndef X11BACKEND_HPP
#define X11BACKEND_HPP

#include <QPixmap>

class PlatformBackend {
    public:
    QPixmap getCursor();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // X11BACKEND_HPP
