#include "imguruploader.hpp"
#include "imgursettingsdialog.hpp"

#include <QBuffer>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <formats.hpp>
#include <io/ioutils.hpp>
#include <notifications.hpp>
#include <screenshotutil.hpp>
#include <settings.hpp>

struct SegfaultWorkaround { // I'm a scrub for doing this
    SegfaultWorkaround(QByteArray a, ImgurUploader *u, QString m) : byteArray(), dis(u), mime(m) {
        a.swap(byteArray);
        QJsonObject object;
        object.insert("client_id", settings::settings().value("imgur/cid").toString());
        object.insert("client_secret", settings::settings().value("imgur/csecret").toString());
        object.insert("grant_type", "refresh_token");
        object.insert("refresh_token", settings::settings().value("imgur/refresh").toString());

        ioutils::postJson(
        QUrl("https://api.imgur.com/oauth2/token"),
        QList<QPair<QString, QString>>({ QPair<QString, QString>("Content-Type", "applicaton/json") }),
        QJsonDocument::fromVariant(object.toVariantMap()).toJson(), [&](QJsonDocument response, QByteArray, QNetworkReply *r) {
            qDebug() << response;
            if (r->error() != QNetworkReply::NoError || !response.isObject()) {
                dis->handleSend(QStringLiteral("Client-ID 8a98f183fc895da"), mime, byteArray);
                return;
            }
            QJsonObject res = response.object();
            if (res.value("success").toBool()) {
                dis->handleSend(QStringLiteral("Client-ID 8a98f183fc895da"), mime, byteArray);
                return;
            }

            QString token = res["access_token"].toString();
            settings::settings().setValue("imgur/expire", QDateTime::currentDateTimeUtc().addSecs(res["expires_in"].toInt()));
            settings::settings().setValue("imgur/refresh", res["refresh_token"].toString());
            settings::settings().setValue("imgur/access", token);

            dis->handleSend(token.prepend("Bearer "), mime, byteArray);
            QScopedPointer<SegfaultWorkaround>(this);
        });
    }

private:
    QByteArray byteArray;
    ImgurUploader *dis;
    QString mime;
};

void ImgurUploader::doUpload(QByteArray byteArray, QString format) {
    if (byteArray.size() > 1e+7) {
        notifications::notify(tr("KShare imgur Uploader"), tr("Failed upload! Image too big"));
        return;
    }
    QString mime;
    if (formats::normalFormatFromName(format) != formats::Normal::None)
        mime = formats::normalFormatMIME(formats::normalFormatFromName(format));
    else
        mime = formats::recordingFormatMIME(formats::recordingFormatFromName(format));
    if (settings::settings().contains("imgur/expire")     //
        && settings::settings().contains("imgur/refresh") //
        && settings::settings().contains("imgur/access")) {
        QDateTime expireTime = settings::settings().value("imgur/expire").toDateTime();
        if (QDateTime::currentDateTimeUtc() > expireTime) {
            new SegfaultWorkaround(byteArray, this, mime);
        } else
            handleSend("Bearer " + settings::settings().value("imgur/access").toString(), mime, byteArray);
    } else
        handleSend(QStringLiteral("Client-ID 8a98f183fc895da"), mime, byteArray);
}

void ImgurUploader::showSettings() {
    (new ImgurSettingsDialog())->show();
}

void ImgurUploader::handleSend(QString auth, QString mime, QByteArray byteArray) {
    ioutils::postJson(QUrl("https://api.imgur.com/3/image"),
                      QList<QPair<QString, QString>>() << QPair<QString, QString>("Content-Type", mime.toUtf8())
                                                       << QPair<QString, QString>("Authorization", auth),
                      byteArray, [byteArray, this, mime](QJsonDocument res, QByteArray, QNetworkReply *r) {
                          QString result = res.object()["data"].toObject()["link"].toString();
                          if (r->error() == QNetworkReply::ContentAccessDenied) {
                              new SegfaultWorkaround(byteArray, this, mime);
                              return;
                          }
                          if (!result.isEmpty()) {
                              screenshotutil::toClipboard(result);
                              notifications::notify(tr("KShare imgur Uploader"), tr("Uploaded to imgur!"));
                          } else {
                              notifications::notify(tr("KShare imgur Uploader "),
                                                    QString(tr("Failed upload! imgur said: HTTP %1: %2"))
                                                    .arg(r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
                                                    .arg(r->errorString()));
                          }
                      });
}
