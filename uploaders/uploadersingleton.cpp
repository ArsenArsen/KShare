#include "uploadersingleton.hpp"
#include "customuploader.hpp"
#include "default/clipboarduploader.hpp"
#include "default/imguruploader.hpp"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <formatter.hpp>
#include <settings.hpp>

UploaderSingleton::UploaderSingleton()
{
    QDir configDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
    configDir.mkpath("KShare/uploaders");
    configDir.cd("KShare/uploaders");
    configDir.setNameFilters({ "*.uploader" });
    for (QString file : configDir.entryList())
    {
        try
        {
            registerUploader(new CustomUploader(configDir.absoluteFilePath(file)));
        }
        catch (std::runtime_error e)
        {
            qWarning() << e.what(); // u wot m8
        }
    }

    // UPLOADERS
    registerUploader(new ImgurUploader);
    registerUploader(new ClipboardUploader);
    // ---------

    if (settings::settings().contains("uploader"))
        uploader = settings::settings().value("uploader").toString();
    else
        settings::settings().setValue("uploader", uploader);
    if (!uploaders.contains(uploader))
    {
        settings::settings().setValue("uploader", uploader);
        uploader = "imgur";
    }
}

void UploaderSingleton::registerUploader(Uploader *uploader)
{
    uploaders.insert(uploader->name(), uploader);
}

void UploaderSingleton::upload(QPixmap *pixmap)
{
    if (settings::settings().contains("fileFormat"))
    {
        QString format = settings::settings().value("fileFormat").toString();
        if (!format.isEmpty())
        {
            pixmap->save(QDir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)).absoluteFilePath(formatter::format(format) + ".png"),
                         "PNG");
        }
    }
    uploaders.value(uploader)->doUpload(pixmap);
}

QList<Uploader *> UploaderSingleton::uploaderList()
{
    return uploaders.values();
}

void UploaderSingleton::set(QString uploader)
{
    if (uploaders.contains(uploader))
    {
        this->uploader = uploader;
        settings::settings().setValue("uploader", uploader);
    }
}

QString UploaderSingleton::selectedUploader()
{
    return uploader;
}
