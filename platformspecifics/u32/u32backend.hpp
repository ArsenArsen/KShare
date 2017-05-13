#ifndef U32BACKEND_HPP
#define U32BACKEND_HPP

#include <QPixmap>

class PlatformBackend {
    public:
    std::tuple<QPoint, QPixmap> getCursor();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // U32BACKEND_HPP
