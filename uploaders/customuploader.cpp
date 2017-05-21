#include "customuploader.hpp"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QNetworkReply>
#include <io/ioutils.hpp>
#include <notifications.hpp>

using std::runtime_error;

void error(QString absFilePath, QString err) {
    throw runtime_error((QString("Invalid file: ").append(absFilePath) + ": " + err).toStdString());
}

CustomUploader::CustomUploader(QString absFilePath) {
    types.insert("PNG", "image/png"); // This is a list of supported formats, too
    types.insert("GIF", "image/gif");
    types.insert("JPG", "image/jpeg");
    types.insert("JPEG", "image/jpeg");
    types.insert("WEBM", "video/webm");
    types.insert("MP4", "video/mp4");
    // Let's go
    QFile file(absFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) error(absFilePath, file.errorString());
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        error(absFilePath, "Root not an object");
    }
    QJsonObject obj = doc.object();
    if (!obj["name"].isString())
        error(absFilePath, "name is not a string");
    else
        uName = obj["name"].toString();
    if (!obj.contains("desc")) {
        if (!obj["desc"].isString())
            /*t*/ error(absFilePath, "desc not a string");
        else
            desc = obj["desc"].toString();
    } else
        desc = absFilePath;
    QJsonValue m = obj["method"];
    if (!m.isUndefined() && !m.isNull()) {
        if (!m.isString()) error(absFilePath, "method not a string");
        QString toCheck = m.toString().toLower();
        if (toCheck == "post")
            method = HttpMethod::POST;
        else
            error(absFilePath, "method invalid");
    }
    QJsonValue url = obj["target"];
    if (!url.isString()) {
        error(absFilePath, "target missing");
    }
    QUrl target(url.toString());
    if (!target.isValid()) error(absFilePath, "target not URL");
    this->target = target;
    QJsonValue formatValue = obj["format"];
    if (!formatValue.isUndefined() && !formatValue.isNull()) {
        if (formatValue.isString()) {
            QString formatString = formatValue.toString().toLower();
            if (formatString == "x-www-form-urlencoded")
                format = RequestFormat::X_WWW_FORM_URLENCODED;
            else if (formatString == "json")
                format = RequestFormat::JSON;
            else if (formatString == "plain")
                format = RequestFormat::PLAIN;
            else
                error(absFilePath, "format invalid");
        }
    } else
        error(absFilePath, "format provided but not string");
    QJsonValue imageValue = obj["imageformat"];
    if (!imageValue.isString()) {
        error(absFilePath, "imageformat not string");
    }
    QString imageFormat = imageValue.toString();
    if (imageFormat == "base64" || QRegExp("base64\\([^+]+\\+[^+]+)").exactMatch(imageFormat)
        || QRegExp("[^+]+\\+[^+]+").exactMatch(imageFormat)) {
        this->iFormat = imageFormat;
    } else
        error(absFilePath, "imageformat invalid");
    QJsonValue bodyValue = obj["body"];
    if (format != RequestFormat::PLAIN) {
        if (bodyValue.isUndefined()) error(absFilePath, "body not set");
        if (bodyValue.isObject())
            body = bodyValue;
        else
            error(absFilePath, "body not object");
    } else {
        if (bodyValue.isString()) {
            body = bodyValue;
        } else
            error(absFilePath, "body not string (reason: format: PLAIN)");
    }
    QJsonValue headerVal = obj["headers"];
    if (!(headerVal.isUndefined() || headerVal.isNull())) {
        if (!headerVal.isObject()) error(absFilePath, "headers must be object");
        headers = headerVal.toObject();
    } else
        headers = QJsonObject();
    QJsonValue returnPsVal = obj["return"];
    if (returnPsVal.isString()) {
        returnPathspec = returnPsVal.toString();
    } else
        error(absFilePath, "return invalid");
    QJsonValue fileLimit = obj["fileLimit"];
    if (!fileLimit.isNull() && !fileLimit.isUndefined()) {
        if (!fileLimit.isDouble()) error(absFilePath, "fileLimit not double");
        limit = fileLimit.toDouble();
    }
}

QString CustomUploader::name() {
    return uName;
}

QString CustomUploader::description() {
    return desc;
}

QString getCType(RequestFormat format, QString plainType) {
    switch (format) {
    case RequestFormat::X_WWW_FORM_URLENCODED:
        return "application/x-www-form-urlencoded";
    case RequestFormat::JSON:
        return "application/json";
    case RequestFormat::PLAIN:
        return plainType;
    }
    return plainType;
}

QList<QPair<QString, QString>> getHeaders(QJsonObject h, QString imageFormat, QMap<QString, QString> types, RequestFormat format) {
    QList<QPair<QString, QString>> headers;
    for (QString s : h.keys()) {
        if (s.toLower() == "content-type") continue;
        QJsonValue v = h[s];
        if (!v.isString())
            headers << QPair<QString, QString>(s, QJsonDocument::fromVariant(v.toVariant()).toJson());
        else
            headers << QPair<QString, QString>(s, v.toString());
    }
    headers << QPair<QString, QString>("Content-Type", getCType(format, types.value(imageFormat)));
    return headers;
}

QByteArray imageBytes(QPixmap *pixmap, QString format) {
    QByteArray returnVal;
    QBuffer buff(&returnVal);
    buff.open(QIODevice::WriteOnly);
    pixmap->save(&buff, format.toUpper().toLocal8Bit().constData());
    return returnVal;
}

QString CustomUploader::getFormatString(bool animated) {
    if (iFormat == "base64")
        return animated ? "GIF" : "PNG";
    else if (QRegExp("[^+]+\\+[^+]+").exactMatch(iFormat))
        return iFormat.split('+')[(int)animated];
    else if (QRegExp("base64\\([^+]+\\+[^+]+)").exactMatch(iFormat))
        return iFormat.mid(7, iFormat.length() - 8).split('+')[(int)animated];
    return "";
}

QJsonObject recurseAndReplace(QJsonObject &body, QByteArray &data, QString contentType) {
    QJsonObject o;
    for (QString s : body.keys()) {
        QJsonValue v = body[s];
        if (v.isObject()) {
            QJsonObject vo = v.toObject();
            o.insert(s, recurseAndReplace(vo, data, contentType));
        } else if (v.isString()) {
            QString str = v.toString();
            if (str.startsWith("/") && str.endsWith("/")) {
                o.insert(s, str.replace("%image", data).replace("%contenttype", contentType));
            }
        }
    }
    return o;
}

QString parsePathspec(QJsonDocument &response, QString &pathspec) {
    if (!pathspec.startsWith(".")) {
        // Does not point to anything
        return "";
    } else {
        if (!response.isObject()) return "";
        QStringList fields = pathspec.right(pathspec.length() - 1).split('.', QString::SkipEmptyParts);
        QJsonObject o = response.object();
        if (pathspec == ".") {
            return QString::fromUtf8(response.toJson());
        }
        QJsonValue val = o[fields.at(0)];
        if (val.isUndefined() || val.isNull())
            return "";
        else if (val.isString())
            return val.toString();
        else if (!val.isObject())
            return QString::fromUtf8(QJsonDocument::fromVariant(val.toVariant()).toJson());
        for (int i = 1; i < fields.size(); i++) {
            if (val.isUndefined() || val.isNull())
                return "";
            else if (val.isString())
                return val.toString();
            else if (!val.isObject())
                return QString::fromUtf8(QJsonDocument::fromVariant(val.toVariant()).toJson());
            else
                val = val.toObject()[fields.at(i)];
        }
        if (val.isUndefined() || val.isNull())
            return "";
        else if (val.isString())
            return val.toString();
        else if (!val.isObject())
            return QString::fromUtf8(QJsonDocument::fromVariant(val.toVariant()).toJson());
    }
    return "";
}

void parseResult(QJsonDocument result, QByteArray data, QString returnPathspec, QString name) {
    if (result.isObject()) {
        qDebug() << result.object()[".url"];
        QString url = parsePathspec(result, returnPathspec);
        if (!url.isEmpty()) {
            QApplication::clipboard()->setText(url);
            notifications::notify("KShare Custom Uploader " + name, "Copied upload link to clipboard!");
        } else
            notifications::notify("KShare Custom Uploader " + name, "Upload done, but result empty!");
    } else {
        notifications::notify("KShare Custom Uploader " + name,
                              "Upload done, but result is not JSON Object! Result in clipboard.");
        QApplication::clipboard()->setText(data);
    }
}

void CustomUploader::doUpload(QPixmap *pixmap) {
    auto h = getHeaders(headers, getFormatString(false), types, this->format);
    QString format = getFormatString(false); // Soon:tm:
    QByteArray data;
    QByteArray imgData = imageBytes(pixmap, format);
    if (iFormat == "base64" || QRegExp("base64\\([^+]\\+[^+]\\)").exactMatch(iFormat)) imgData = imgData.toBase64();
    switch (this->format) {
    case RequestFormat::PLAIN: {
        data = imgData;
    } break;
    case RequestFormat::JSON: {
        if (body.isString()) {
            QStringList split = body.toString().replace("%contenttype", types.value(format)).split("%imagedata");
            for (int i = 0; i < split.size(); i++) {
                data.append(split[i]);
                if (i < split.size() - 1) data.append(imgData);
            }
        } else {
            QJsonObject vo = body.toObject();
            data = QJsonDocument::fromVariant(recurseAndReplace(vo, imgData, types.value(format)).toVariantMap()).toJson();
        }
    } break;
    case RequestFormat::X_WWW_FORM_URLENCODED: {
        QJsonObject body = this->body.toObject();
        for (QString key : body.keys()) {
            QJsonValue val = body[key];
            if (val.isString()) {
                QString str = val.toString();
                QByteArray strB;
                if (str.startsWith("/") && str.endsWith("/")) {
                    str = str.mid(1, str.length() - 2);
                    QStringList split = str.replace("%contenttype", types.value(format)).split("%imagedata");
                    for (int i = 0; i < split.size(); i++) {
                        strB.append(split[i]);
                        if (i < split.size() - 1) strB.append(imgData);
                    }
                }
                data.append(QUrl::toPercentEncoding(key)).append('=').append(strB);
            } else {
                if (!data.isEmpty()) data.append('&');
                data.append(QUrl::toPercentEncoding(key))
                .append('=')
                .append(QUrl::toPercentEncoding(QJsonDocument::fromVariant(body[key].toVariant()).toJson()));
            }
        }
    } break;
    }
    if (limit > 0 && data.size() > limit) {
        notifications::notify("KShare Custom Uploader " + name(), "File limit exceeded!");
        return;
    }
    switch (method) {
    case HttpMethod::POST:
        if (returnPathspec == "|") {
            ioutils::postData(target, h, data, [&](QByteArray result, QNetworkReply *) {
                QApplication::clipboard()->setText(QString::fromUtf8(result));
                notifications::notify("KShare Custom Uploader " + name(), "Copied upload result to clipboard!");
            });
        } else {
            ioutils::postJson(target, h, data, [&](QJsonDocument result, QByteArray data, QNetworkReply *) {
                parseResult(result, data, returnPathspec, name());
            });
        }
        break;
    }
}
