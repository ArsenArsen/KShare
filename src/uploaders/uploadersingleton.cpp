#include "uploadersingleton.hpp"
#include "customuploader.hpp"
#include "default/clipboarduploader.hpp"
#include "default/imguruploader.hpp"
#include <QBuffer>
#include <logger.hpp>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <formats.hpp>
#include <formatter.hpp>
#include <notifications.hpp>
#include <settings.hpp>
#include <uploaders/default/imgplusuploader.hpp>

UploaderSingleton::UploaderSingleton() : QObject() {
    switch (settings::settings().value("saveLocation", 1).toInt()) {
    case 0:
        saveDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        if (QStandardPaths::writableLocation(QStandardPaths::PicturesLocation).isEmpty()) {
            qFatal("%s", tr("Cannot determine location for pictures").toLocal8Bit().constData());
        }
        break;
    case 1:
        if (QStandardPaths::writableLocation(QStandardPaths::HomeLocation).isEmpty()) {
            qFatal("%s", tr("Cannot determine location of your home directory").toLocal8Bit().constData());
        }
        saveDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Screenshots";
        break;
    default:
        qFatal("%s", tr("Invalid config [saveLocation not int or is not in range]").toLocal8Bit().constData());
    }

    if (!saveDir.exists()) {
        if (!saveDir.mkpath(".")) {
            qFatal("Could not create the path %s to store images in!", saveDir.absolutePath().toLocal8Bit().constData());
        }
    }
    QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    configDir.mkpath("KShare/uploaders");
    configDir.cd("KShare/uploaders");
    configDir.setNameFilters({ "*.uploader" });
    for (QString file : configDir.entryList()) {
        try {
            registerUploader(new CustomUploader(configDir.absoluteFilePath(file)));
        } catch (std::runtime_error &e) {
            logger::warn(QString::fromStdString(e.what()));
            errs << e;
        }
    }

    // UPLOADERS
    registerUploader(new ImgurUploader);
    registerUploader(new ImgplusUploader);
    registerUploader(new ClipboardUploader);
    // ---------

    if (settings::settings().contains("uploader"))
        uploader = settings::settings().value("uploader").toString();
    else
        settings::settings().setValue("uploader", uploader);
    if (!uploaders.contains(uploader)) {
        uploader = "imgur";
        settings::settings().setValue("uploader", uploader);
    }
}

void UploaderSingleton::registerUploader(Uploader *uploader) {
    if (uploaders.contains(uploader->name()))
        throw std::runtime_error((tr("Ambigious uploader ") + uploader->name()).toStdString());
    uploaders.insert(uploader->name(), uploader);
    emit newUploader(uploader);
}

void UploaderSingleton::upload(QPixmap pixmap) {
    auto u = uploaders.value(uploader);
    if (!u->validate()) {
        u = uploaders.value("imgur");
        set("imgur");
        logger::warn(tr("Currently selected uploader is not set up properly! Falling back to imgur"));
    }
    QString format = settings::settings().value("captureformat", "PNG").toString();
    QFile file(saveDir.absoluteFilePath(
    formatter::format(settings::settings().value("fileFormat", "Screenshot %(yyyy-MM-dd HH-mm-ss)date.%ext").toString(),
                      format.toLower())));

    if (file.open(QFile::ReadWrite)) {
        pixmap.save(&file, format.toLocal8Bit().constData(), settings::settings().value("imageQuality", -1).toInt());
        file.seek(0);
        u->doUpload(file.readAll(), format);
    } else
        notifications::notify(tr("KShare - Failed to save picture"), file.errorString(), QSystemTrayIcon::Warning);
}

void UploaderSingleton::upload(QByteArray img, QString format) {
    if (img.isEmpty()) return;
    QFile file(saveDir.absoluteFilePath(
    formatter::format(settings::settings().value("fileFormat", "Screenshot %(yyyy-MM-dd HH-mm-ss)date.%ext").toString(),
                      format.toLower())));
    if (file.open(QFile::WriteOnly)) {
        file.write(img);
        file.close();
    }
    uploaders.value(uploader)->doUpload(img, format);
}

void UploaderSingleton::upload(QFile &img, QString format) {
    if (img.size() <= 0) return;
    if (img.rename(saveDir.absoluteFilePath(
        formatter::format(settings::settings().value("fileFormat", "Screenshot %(yyyy-MM-dd HH-mm-ss)date.%ext").toString(),
                          format.toLower())))) {
        if (img.open(QFile::ReadWrite))
            uploaders.value(uploader)->doUpload(img.readAll(), format);
        else
            notifications::notify(tr("KShare - Failed to save picture"), img.errorString(), QSystemTrayIcon::Warning);
    } else
        notifications::notify(tr("KShare - Failed to save picture"), img.errorString(), QSystemTrayIcon::Warning);
}

void UploaderSingleton::showSettings() {
    uploaders.value(uploader)->showSettings();
}

bool UploaderSingleton::validate() {
    return uploaders.value(uploader)->validate();
}

QList<Uploader *> UploaderSingleton::uploaderList() {
    return uploaders.values();
}

void UploaderSingleton::set(QString uploader) {
    if (uploaders.contains(uploader)) {
        this->uploader = uploader;
        settings::settings().setValue("uploader", uploader);
        emit uploaderChanged(uploader);
    }
}

QString UploaderSingleton::selectedUploader() {
    return uploader;
}

QList<std::runtime_error> UploaderSingleton::errors() {
    return errs;
}

QString UploaderSingleton::currentUploader() {
    return uploader;
}
