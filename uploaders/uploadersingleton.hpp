#ifndef UPLOADERSINGLETON_HPP
#define UPLOADERSINGLETON_HPP

#include "uploader.hpp"
#include <QMap>

class UploaderSingleton
{
    public:
    static UploaderSingleton &inst()
    {
        static UploaderSingleton inst;
        return inst;
    }
    void registerUploader(Uploader *uploader);
    void upload(QPixmap *pixmap);
    QList<Uploader *> uploaderList();
    void set(QString uploader);
    QString selectedUploader();
    QList<std::runtime_error> errors();

    private:
    UploaderSingleton();
    QMap<QString, Uploader *> uploaders;
    QString uploader = "imgur";
    QList<std::runtime_error> errs;
};

#endif // UPLOADERSINGLETON_HPP
