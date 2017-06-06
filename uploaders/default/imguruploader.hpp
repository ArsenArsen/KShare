#ifndef IMGURUPLOADER_HPP
#define IMGURUPLOADER_HPP

#include "../uploader.hpp"

class ImgurUploader : public Uploader {
public:
    QString name() {
        return "imgur";
    }
    QString description() {
        return "imgur.com uploader";
    }
    void doUpload(QByteArray byteArray, QString);
};

#endif // IMGURUPLOADER_HPP
