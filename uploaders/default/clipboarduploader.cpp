#include "clipboarduploader.hpp"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <formats.hpp>
#include <notifications.hpp>

void ClipboardUploader::doUpload(QByteArray imgData, QString format) {
    auto f = formats::recordingFormatFromName(format);
    if (f != formats::Recording::None) {
        auto data = new QMimeData();
        data->setData(formats::recordingFormatMIME(f), imgData);
        QApplication::clipboard()->setMimeData(data);
    }
    QApplication::clipboard()->setImage(QImage::fromData(imgData, format.toLocal8Bit().constData()));
    notifications::notify("KShare", "Copied to clipboard!");
}
