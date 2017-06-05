#ifndef MACBACKEND_HPP
#define MACBACKEND_HPP

#include <QPixmap>

class PlatformBackend {
public:
    QPixmap getCursor();
    pid_t pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
};

#endif // MACBACKEND_HPP
