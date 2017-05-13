#include "macbackend.hpp"

QPixmap PlatformBackend::getCursor() {
#warning "TODO: Mac backend"
    return std::tuple<QPoint, QPixmap>(QPoint(0, 0), QPixmap());
}
