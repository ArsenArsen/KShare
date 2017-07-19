#include "customuploader.hpp"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <formats.hpp>
#include <io/ioutils.hpp>
#include <notifications.hpp>

using formats::normalFormatFromName;
using formats::normalFormatMIME;
using formats::recordingFormatFromName;
using formats::recordingFormatMIME;
using std::runtime_error;

void error(QString absFilePath, QString err) {
    throw runtime_error((QString("Invalid file: ").append(absFilePath) + ": " + err).toStdString());
}

CustomUploader::CustomUploader(QString absFilePath) {
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
                rFormat = RequestFormat::X_WWW_FORM_URLENCODED;
            else if (formatString == "json")
                rFormat = RequestFormat::JSON;
            else if (formatString == "plain")
                rFormat = RequestFormat::PLAIN;
            else if (formatString == "multipart-form-data")
                rFormat = RequestFormat::MULTIPART_FORM_DATA;
            else
                error(absFilePath, "format invalid");
        }
    } else
        error(absFilePath, "format provided but not string");
    QJsonValue bodyValue = obj["body"];
    if (rFormat != RequestFormat::PLAIN) {
        if (bodyValue.isUndefined()) error(absFilePath, "body not set");
        if (rFormat == RequestFormat::MULTIPART_FORM_DATA) {
            if (bodyValue.isArray()) {
                for (QJsonValue val : bodyValue.toArray()) {
                    if (!val.isObject()) error(absFilePath, "all elements of body must be objects");
                    if (!val.toObject()["body"].isObject() && !val.toObject().value("body").isString())
                        error(absFilePath, "all parts must have a body which is object or string!");
                    QJsonObject vo = val.toObject();
                    for (auto v : vo["body"].toObject())
                        if (!v.isObject() && !v.isString())
                            error(absFilePath, "all parts of body must be string or object");
                    for (auto v : vo.keys())
                        if (v.startsWith("__") && !vo[v].isString())
                            error(absFilePath, "all __headers must be strings");
                }
                body = bodyValue;
            } else
                error(absFilePath, "body not array (needed for multipart)");
        } else {
            if (bodyValue.isObject())
                body = bodyValue;
            else
                error(absFilePath, "body not object");
        }
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
    QJsonValue bool64 = obj["base64"];
    if (!bool64.isNull() && !bool64.isUndefined()) {
        if (!bool64.isBool()) error(absFilePath, "base64 must be boolean");
        base64 = bool64.toBool();
        if (rFormat == RequestFormat::JSON && !base64) error(absFilePath, "base64 required with json");
    }
    urlPrepend = obj["return_prepend"].toString();
    urlAppend = obj["return_append"].toString();
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
    case RequestFormat::MULTIPART_FORM_DATA:
        return "multipart/form-data";
    case RequestFormat::PLAIN:
        return plainType;
    }
    return plainType;
}

QList<QPair<QString, QString>> getHeaders(QJsonObject h, QString imageFormat, RequestFormat format) {
    QList<QPair<QString, QString>> headers;
    for (QString s : h.keys()) {
        if (s.toLower() == "content-type") continue;
        QJsonValue v = h[s];
        if (!v.isString())
            headers << QPair<QString, QString>(s, QJsonDocument::fromVariant(v.toVariant()).toJson());
        else
            headers << QPair<QString, QString>(s, v.toString());
    }
    headers << QPair<QString, QString>("Content-Type", getCType(format, normalFormatMIME(normalFormatFromName(imageFormat))));
    return headers;
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
            else if (val.isArray())
                val = val.toArray()[fields.at(i).toInt()];
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

void parseResult(QJsonDocument result, QByteArray data, QString returnPathspec, QString name, QString urlPrepend, QString urlAppend) {
    if (result.isObject()) {
        QString url = urlPrepend + parsePathspec(result, returnPathspec) + urlAppend;
        if (!url.isEmpty()) {
            QApplication::clipboard()->setText(url);
            notifications::notify("KShare Custom Uploader " + name, "Copied upload link to clipboard!");
        } else {
            notifications::notify("KShare Custom Uploader " + name, "Upload done, but result empty!");
            QApplication::clipboard()->setText(data);
        }
    } else {
        notifications::notify("KShare Custom Uploader " + name,
                              "Upload done, but result is not JSON Object! Result in clipboard.");
        QApplication::clipboard()->setText(data);
    }
}

QByteArray substituteArgs(QByteArray arr, QString format, QByteArray imgData = QByteArray()) {
    QString mime = normalFormatMIME(normalFormatFromName(format));
    if (mime.isEmpty()) mime = recordingFormatMIME(recordingFormatFromName(format));
    if (arr.startsWith("/") && arr.endsWith("/")) {
        arr = arr.mid(1, arr.length() - 2);

        arr = arr.replace("%contenttype", mime.toUtf8());
        arr = arr.replace("%FORMAT", format.toUpper().toUtf8());
        arr = arr.replace("%format", format.toLower().toUtf8());

        if (imgData.isNull()) return arr;
        return arr.replace("%imagedata", imgData);
    } else
        return arr;
}


QJsonObject recurseAndReplace(QJsonObject &body, QByteArray &data, QString format) {
    QJsonObject o;
    for (QString s : body.keys()) {
        QJsonValue v = body[s];
        if (v.isObject()) {
            QJsonObject vo = v.toObject();
            o.insert(s, recurseAndReplace(vo, data, format));
        } else if (v.isString()) {
            QString str = v.toString();
            if (str.startsWith("/") && str.endsWith("/")) {
                o.insert(s, QString::fromUtf8(substituteArgs(str.toUtf8(), format, data)));
            } else
                o.insert(s, v);
        } else
            o.insert(s, v);
    }
    return o;
}

void CustomUploader::doUpload(QByteArray imgData, QString format) {
    auto h = getHeaders(headers, format, this->rFormat);
    QByteArray data;
    if (base64) imgData = imgData.toBase64();

    switch (this->rFormat) {
    case RequestFormat::PLAIN: {
        data = imgData;
    } break;
    case RequestFormat::JSON: {
        if (body.isString()) {
            data = substituteArgs(body.toString().toUtf8(), format, imgData);
        } else {
            QJsonObject vo = body.toObject();
            data = QJsonDocument::fromVariant(recurseAndReplace(vo, imgData, format).toVariantMap()).toJson();
        }
    } break;
    case RequestFormat::X_WWW_FORM_URLENCODED: {
        QJsonObject body = this->body.toObject();
        for (QString key : body.keys()) {
            QJsonValue val = body[key];
            if (val.isString()) {
                data.append(QUrl::toPercentEncoding(key)).append('=').append(substituteArgs(val.toString().toUtf8(), format, imgData));
            } else {
                if (!data.isEmpty()) data.append('&');
                data.append(QUrl::toPercentEncoding(key))
                .append('=')
                .append(QUrl::toPercentEncoding(QJsonDocument::fromVariant(body[key].toVariant()).toJson()));
            }
        }
    } break;
    case RequestFormat::MULTIPART_FORM_DATA: {
        QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        auto arr = body.toArray();
        QList<QBuffer *> buffersToDelete;
        QList<QByteArray *> arraysToDelete;
        for (QJsonValue val : arr) {
            auto valo = val.toObject();
            QHttpPart part;
            QJsonValue bd = valo["body"];
            if (bd.isString()) {
                QByteArray body = substituteArgs(bd.toString().toUtf8(), format, imgData);
                QByteArray *bodyHeap = new QByteArray;
                body.swap(*bodyHeap);
                QBuffer *buffer = new QBuffer(bodyHeap);
                buffer->open(QIODevice::ReadOnly);
                part.setBodyDevice(buffer);
                buffersToDelete.append(buffer);
                arraysToDelete.append(bodyHeap);
            } else {
                auto bdo = bd.toObject();
                QJsonObject result = recurseAndReplace(bdo, imgData, format);
                part.setBody(QJsonDocument::fromVariant(result.toVariantMap()).toJson());
            }
            QByteArray cdh("form-data");
            for (QString headerVal : valo.keys()) {
                if (headerVal.startsWith("__")) {
                    headerVal = headerVal.mid(2);
                    QByteArray str = valo[headerVal].toString().toUtf8();
                    if (str.startsWith("/") && str.endsWith("/")) str = substituteArgs(str, format);
                    part.setRawHeader(headerVal.toLatin1(), str);
                } else if (headerVal != "body")
                    cdh += "; " + headerVal + "=\"" + valo[headerVal].toString().replace("\"", "\\\"") + "\"";
            }
            part.setHeader(QNetworkRequest::ContentDispositionHeader, cdh);
            multipart->append(part);
        }
        switch (method) {
        case HttpMethod::POST:
            if (returnPathspec == "|") {
                ioutils::postMultipartData(target, h, multipart, [&, buffersToDelete, arraysToDelete](QByteArray result, QNetworkReply *) {
                    QApplication::clipboard()->setText(QString::fromUtf8(result));
                    for (auto buffer : buffersToDelete) buffer->deleteLater();
                    for (auto arr : arraysToDelete) delete arr;
                    notifications::notify("KShare Custom Uploader " + name(), "Copied upload result to clipboard!");
                });
            } else {
                ioutils::postMultipart(target, h, multipart,
                                       [&, buffersToDelete, arraysToDelete](QJsonDocument result, QByteArray data, QNetworkReply *) {
                                           for (auto buffer : buffersToDelete) buffer->deleteLater();
                                           for (auto arr : arraysToDelete) delete arr;
                                           parseResult(result, data, returnPathspec, name(), urlPrepend, urlPrepend);
                                       });
            }
            break;
        }
        return;
    }
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
                parseResult(result, data, returnPathspec, name(), urlPrepend, urlPrepend);
            });
        }
        break;
    }
}
