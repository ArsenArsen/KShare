#ifndef CLIPBOARDUPLOADER_HPP
#define CLIPBOARDUPLOADER_HPP

#include <QPixmap>
#include <uploaders/uploader.hpp>

class ClipboardUploader : public Uploader {
public:
    QString name() {
        return "clipboard";
    }
    QString description() {
        return "Copies the image to clipboard";
    }
    std::tuple<QString, QString> format() {
        return std::tuple<QString, QString>("PNG", "MP4");
    }

    void doUpload(QByteArray imgData);
};

#endif // CLIPBOARDUPLOADER_HPP
