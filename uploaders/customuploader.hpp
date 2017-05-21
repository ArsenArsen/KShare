#ifndef CUSTOMUPLOADER_HPP
#define CUSTOMUPLOADER_HPP

#include "uploader.hpp"
#include <QJsonObject>
#include <QMap>
#include <QUrl>

enum class HttpMethod { POST };

enum class RequestFormat { X_WWW_FORM_URLENCODED, JSON, PLAIN };

class CustomUploader : public Uploader {
    public:
    CustomUploader(QString absFilePath);
    QString name();
    QString description();
    void doUpload(QPixmap *pixmap);
    QString getFormatString(bool animated);
    QMap<QString, QString> types;

    private:
    double limit = -1;
    QString desc;
    QString uName;
    RequestFormat format = RequestFormat::JSON;
    HttpMethod method = HttpMethod::POST;
    QUrl target;
    QJsonValue body;
    QJsonObject headers;
    QString returnPathspec;
    QString iFormat;
};

#endif // CUSTOMUPLOADER_HPP
