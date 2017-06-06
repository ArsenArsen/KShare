#include "settings.hpp"

#include <QMutex>
#include <QStandardPaths>

QMutex *lock = new QMutex;

QSettings &settings::settings() {
    QMutexLocker l(lock);
    static QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    if (configDir.dirName() != "KShare") {
        configDir.mkdir("KShare");
        configDir.cd("KShare");
    }
    static QSettings settings(configDir.absoluteFilePath("settings.ini"), QSettings::IniFormat);
    return settings;
}
