#include "imguruploader.hpp"

#include <QBuffer>
#include <QJsonObject>
#include <QJsonValue>
#include <io/ioutils.hpp>
#include <notifications.hpp>
#include <screenshotutil.hpp>

void ImgurUploader::doUpload(QPixmap *pixmap) {
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  pixmap->save(&buffer, "PNG");
  ioutils::postJson(
      QUrl("https://api.imgur.com/3/image"),
      QList<QPair<QString, QString>>()
          << QPair<QString, QString>("Content-Type",
                                     "application/x-www-form-urlencoded")
          << QPair<QString, QString>("Authorization",
                                     "Client-ID 8a98f183fc895da"),
      byteArray, [](QJsonDocument res, QNetworkReply *) {
        QString result = res.object()["data"].toObject()["link"].toString();
        screenshotutil::toClipboard(result);
        notifications::notify("KShare imgur Uploader ",
                              result.isEmpty()
                                  ? "Failed upload!"
                                  : "Upload done, but result empty!");
      });
}
