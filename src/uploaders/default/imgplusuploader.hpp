#ifndef IMGPLUSUPLOADER_HPP
#define IMGPLUSUPLOADER_HPP

#include <QApplication>
#include <uploaders/uploader.hpp>

class ImgplusUploader : public Uploader {
    Q_DECLARE_TR_FUNCTIONS(ImgplusUploader)
public:
    QString name() override {
        return "ImagePlus";
    }
    QString description() override {
        return "imgpl.us uploader";
    }
    void doUpload(QByteArray byteArray, QString format) override;
    void showSettings() override;
    bool validate() override;
};

#endif // IMGPLUSUPLOADER_HPP
