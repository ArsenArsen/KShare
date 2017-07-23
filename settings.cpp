#include "settings.hpp"

#include <QDebug>
#include <QMutex>
#include <QStandardPaths>

QSettings &settings::settings() {
    QMutexLocker l(lock);
    static QSettings settings(dir().absoluteFilePath("settings.ini"), QSettings::IniFormat);
    return settings;
}

QDir settings::dir() {
    static QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    if (configDir.dirName() != "KShare") {
        if (!configDir.cd("KShare")) {
            if (!configDir.mkdir("KShare")) {
                qFatal("Could not make config directory");
            } else {
                configDir.cd("KShare");
            }
        }
    }
    return configDir;
}
