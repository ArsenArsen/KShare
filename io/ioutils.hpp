#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <functional>

namespace ioutils
{
extern QNetworkAccessManager networkManager;
void getJson(QUrl target, QList<QPair<QString, QString>> headers, std::function<void(QJsonDocument)> callback);
void postJson(QUrl target,
              QList<QPair<QString, QString>> headers,
              QByteArray body,
              std::function<void(QJsonDocument)> callback);
// If I need more I will add
// Maybe when people start with plugins and custom uploaders
// Wait, that's a secret
}

#endif // IOUTILS_HPP
