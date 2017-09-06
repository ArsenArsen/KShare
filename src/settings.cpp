#include "settings.hpp"

#include <QDebug>
#include <QStandardPaths>

QSettings &settings::settings() {
    static QSettings settings(dir().absoluteFilePath("settings.ini"), QSettings::IniFormat);
    return settings;
}

QDir settings::dir() {
    static QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    if (configDir.dirName() != "KShare") {
        if (!configDir.cd("KShare")) {
            if (!configDir.mkdir("KShare")) {
                qFatal("%s", QObject::tr("Could not make config directory").toLocal8Bit().constData());
            } else {
                configDir.cd("KShare");
            }
        }
    }
    return configDir;
}
