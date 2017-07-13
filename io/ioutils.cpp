#include "ioutils.hpp"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

QNetworkAccessManager ioutils::networkManager;

void ioutils::postMultipart(QUrl target,
                            QList<QPair<QString, QString>> headers,
                            QHttpMultiPart *body,
                            std::function<void(QJsonDocument, QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.post(req, body);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        QByteArray data = reply->readAll();
        callback(QJsonDocument::fromJson(data), data, reply);
        delete reply;
    });
}

void ioutils::postMultipartData(QUrl target,
                                QList<QPair<QString, QString>> headers,
                                QHttpMultiPart *body,
                                std::function<void(QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        if (header.first.toLower() != "content-type") req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.post(req, body);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        callback(reply->readAll(), reply);
        delete reply;
    });
}

void ioutils::getJson(QUrl target,
                      QList<QPair<QString, QString>> headers,
                      std::function<void(QJsonDocument, QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.get(req);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        QByteArray data = reply->readAll();
        callback(QJsonDocument::fromJson(data), data, reply);
        reply->deleteLater();
    });
}

void ioutils::postJson(QUrl target,
                       QList<QPair<QString, QString>> headers,
                       QByteArray body,
                       std::function<void(QJsonDocument, QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.post(req, body);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        QByteArray data = reply->readAll();
        callback(QJsonDocument::fromJson(data), data, reply);
        delete reply;
    });
}

void ioutils::getData(QUrl target, QList<QPair<QString, QString>> headers, std::function<void(QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.get(req);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        callback(reply->readAll(), reply);
        delete reply;
    });
}

void ioutils::postData(QUrl target,
                       QList<QPair<QString, QString>> headers,
                       QByteArray body,
                       std::function<void(QByteArray, QNetworkReply *)> callback) {
    QNetworkRequest req(target);
    for (auto header : headers) {
        req.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
    QNetworkReply *reply = networkManager.post(req, body);
    QObject::connect(reply, &QNetworkReply::finished, [reply, callback] {
        callback(reply->readAll(), reply);
        delete reply;
    });
}


QString ioutils::methodString(QNetworkAccessManager::Operation operation) {
    switch (operation) {
    case QNetworkAccessManager::GetOperation:
        return "GET";
    case QNetworkAccessManager::PostOperation:
        return "POST";
    case QNetworkAccessManager::PutOperation:
        return "PUT";
    case QNetworkAccessManager::DeleteOperation:
        return "DELETE";
    case QNetworkAccessManager::HeadOperation:
        return "HEAD";
    default:
        //        return "Dunno";
        return "Unknown";
    }
}
