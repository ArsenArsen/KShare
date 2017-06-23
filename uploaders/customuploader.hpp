#ifndef CUSTOMUPLOADER_HPP
#define CUSTOMUPLOADER_HPP

#include "uploader.hpp"
#include <QJsonObject>
#include <QMap>
#include <QUrl>

enum class HttpMethod { POST };

enum class RequestFormat { X_WWW_FORM_URLENCODED, JSON, MULTIPART_FORM_DATA, PLAIN };

class CustomUploader : public Uploader {
public:
    CustomUploader(QString absFilePath);
    QString name();
    QString description();
    void doUpload(QByteArray imgData, QString format);

private:
    double limit = -1;
    QString desc;
    QString uName;
    RequestFormat rFormat = RequestFormat::JSON;
    HttpMethod method = HttpMethod::POST;
    QUrl target;
    QJsonValue body;
    QJsonObject headers;
    bool base64 = false;
    QString returnPathspec;
};

#endif // CUSTOMUPLOADER_HPP
