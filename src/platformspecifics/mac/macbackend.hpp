#ifndef MACBACKEND_HPP
#define MACBACKEND_HPP

#include <QPixmap>

#define PLATFORM_CAPABILITY_PID

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    pid_t pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
    bool filenameValid(QString name);
};

#endif // MACBACKEND_HPP
