#ifndef MACBACKEND_HPP
#define MACBACKEND_HPP

#include <QPixmap>

class PlatformBackend {
    public:
    QPixmap getCursor();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // MACBACKEND_HPP
