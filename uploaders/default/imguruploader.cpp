#include "imguruploader.hpp"

#include <QBuffer>
#include <QJsonObject>
#include <QJsonValue>
#include <io/ioutils.hpp>
#include <notifications.hpp>
#include <screenshotutil.hpp>

void ImgurUploader::doUpload(QByteArray byteArray, QString) {
    if (byteArray.size() > 1e+7) {
        notifications::notify("KShare imgur Uploader ", "Failed upload! Image too big");
        return;
    }
    ioutils::postJson(QUrl("https://api.imgur.com/3/image"),
                      QList<QPair<QString, QString>>()
                      << QPair<QString, QString>("Content-Type", "application/x-www-form-urlencoded")
                      << QPair<QString, QString>("Authorization", "Client-ID 8a98f183fc895da"),
                      byteArray, [](QJsonDocument res, QByteArray, QNetworkReply *) {
                          QString result = res.object()["data"].toObject()["link"].toString();
                          if (!result.isEmpty()) {
                              screenshotutil::toClipboard(result);
                              notifications::notify("KShare imgur Uploader ", "Uploaded to imgur!");
                          } else {
                              notifications::notify("KShare imgur Uploader ",
                                                    QString("Failed upload! imgur said: HTTP %2: %1")
                                                    .arg(res.object()["data"].toObject()["error"].toString())
                                                    .arg(QString::number(res.object()["status"].toInt())));
                          }
                      });
}
