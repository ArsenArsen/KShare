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
    std::tuple<QString, QString> format() {
        return std::tuple<QString, QString>("PNG", "MP4");
    }
    void doUpload(QByteArray byteArray);
};

#endif // IMGURUPLOADER_HPP
