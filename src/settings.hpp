#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QDir>
#include <QSettings>

namespace settings {
    QSettings &settings();
    QDir dir();
} // namespace settings

#endif // SETTINGS_HPP
