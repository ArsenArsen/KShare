#include "clipboarduploader.hpp"

#include <QApplication>
#include <QClipboard>
#include <notifications.hpp>

void ClipboardUploader::doUpload(QByteArray imgData) {
    QApplication::clipboard()->setImage(QImage::fromData(imgData, std::get<0>(format()).toLocal8Bit().constData()));
    notifications::notify("KShare", "Copied to clipboard!");
}
