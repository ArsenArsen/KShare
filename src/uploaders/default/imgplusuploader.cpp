#include "imgplusuploader.hpp"
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QHttpMultiPart>
#include <QInputDialog>
#include <formats.hpp>
#include <io/ioutils.hpp>
#include <notifications.hpp>
#include <settings.hpp>

void ImgplusUploader::doUpload(QByteArray byteArray, QString format) {
    QString mime;
    if (formats::normalFormatFromName(format) != formats::Normal::None)
        mime = formats::normalFormatMIME(formats::normalFormatFromName(format));
    else
        mime = formats::recordingFormatMIME(formats::recordingFormatFromName(format));
    QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"file\"; filename=\"image." + format + "\"");
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, mime);
    QByteArray *data = new QByteArray;
    data->swap(byteArray);
    QBuffer *buff = new QBuffer(data);
    buff->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(buff);
    multipart->append(imagePart);

    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"api_key\"");
    keyPart.setBody(settings::settings().value("imgplus/apikey").toString().toUtf8());
    multipart->append(keyPart);

    ioutils::postMultipartData(QUrl("http://imgpl.us/api/upload"),
                               { QPair<QString, QString>("Accept", "application/json") }, multipart,
                               [](QByteArray link, QNetworkReply *) {
                                   QApplication::clipboard()->setText(QString::fromUtf8(link));
                                   if (!link.startsWith("http"))
                                       qCritical() << QObject::tr("Failed to upload! Copied the response to clipboard");
                                   else
                                       notifications::notify(QObject::tr("KShare imgplus Uploader"),
                                                             QObject::tr("Uploaded to ImagePlus!"));
                               });
}

void ImgplusUploader::showSettings() {
    QString text = QInputDialog::getText(0, tr("imgplus API key"), tr("Enter the imgpl.us API key (Found in Settings)"),
                                         QLineEdit::Normal, settings::settings().value("imgplus/apikey").toString());
    if (!text.isNull()) settings::settings().setValue("imgplus/apikey", text);
}

bool ImgplusUploader::validate() {
    return settings::settings().value("imgplus/apikey").userType() == QMetaType::QString;
}
