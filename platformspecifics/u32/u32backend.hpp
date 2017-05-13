#ifndef U32BACKEND_HPP
#define U32BACKEND_HPP

#include <QPixmap>

class PlatformBackend {
    public:
    QPixmap getCursor();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // U32BACKEND_HPP
