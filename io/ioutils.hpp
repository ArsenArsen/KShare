#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <functional>

namespace ioutils {
extern QNetworkAccessManager networkManager;
void getJson(QUrl target, QList<QPair<QString, QString>> headers, std::function<void(QJsonDocument, QByteArray, QNetworkReply *)> callback);
void postJson(QUrl target,
              QList<QPair<QString, QString>> headers,
              QByteArray body,
              std::function<void(QJsonDocument, QByteArray, QNetworkReply *)> callback);
void getData(QUrl target, QList<QPair<QString, QString>> headers, std::function<void(QByteArray, QNetworkReply *)> callback);
void postData(QUrl target, QList<QPair<QString, QString>> headers, QByteArray body, std::function<void(QByteArray, QNetworkReply *)> callback);
}

#endif // IOUTILS_HPP
