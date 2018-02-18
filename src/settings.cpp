#include "settings.hpp"

#include <QStandardPaths>
#include <logger.hpp>

QSettings &settings::settings() {
    static QSettings settings(dir().absoluteFilePath("settings.ini"), QSettings::IniFormat);
    return settings;
}

QDir settings::dir() {
    static QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    if (configDir.dirName() != "KShare") {
        if (!configDir.cd("KShare")) {
            if (!configDir.mkdir("KShare")) {
                logger::abort(QObject::tr("Could not make config directory"));
            } else {
                configDir.cd("KShare");
            }
        }
    }
    return configDir;
}
