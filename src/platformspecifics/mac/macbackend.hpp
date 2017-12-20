#ifndef MACBACKEND_HPP
#define MACBACKEND_HPP

#include <QPixmap>

#define PLATFORM_CAPABILITY_PID
#define PLATFORM_CAPABILITY_CURRENT_USER

class PlatformBackend {
public:
    std::tuple<QPoint, QPixmap> getCursor();
    pid_t pid();
    static PlatformBackend &inst() {
        static PlatformBackend inst;
        return inst;
    }
    bool filenameValid(QString name);
    QString getCurrentUser();
};

#endif // MACBACKEND_HPP
