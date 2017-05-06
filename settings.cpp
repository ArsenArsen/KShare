#include "settings.hpp"

#include <QStandardPaths>

QSettings &settings::settings() {
  static QDir configDir(
      QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
  if (configDir.path() ==
      QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)) {
    configDir.mkdir("KShare");
    configDir.cd("KShare");
  }
  static QSettings settings(configDir.absoluteFilePath("settings.ini"),
                            QSettings::IniFormat);
  return settings;
}
