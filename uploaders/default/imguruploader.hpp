#ifndef IMGURUPLOADER_HPP
#define IMGURUPLOADER_HPP

#include "../uploader.hpp"
#include <QApplication>

class ImgurUploader : public Uploader {
    Q_DECLARE_TR_FUNCTIONS(ImgurUploader)
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
