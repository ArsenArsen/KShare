#ifndef IMGURUPLOADER_HPP
#define IMGURUPLOADER_HPP

#include "../uploader.hpp"

class ImgurUploader : public Uploader {
    friend struct SegfaultWorkaround;

public:
    QString name() override {
        return "imgur";
    }
    QString description() override {
        return "imgur.com uploader";
    }
    void doUpload(QByteArray byteArray, QString) override;
    void showSettings() override;

private:
    void handleSend(QString auth, QString mime, QByteArray byteArray);
};

#endif // IMGURUPLOADER_HPP
