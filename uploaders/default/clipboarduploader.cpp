#include "clipboarduploader.hpp"

#include <QApplication>
#include <QClipboard>

void ClipboardUploader::doUpload(QPixmap *pixmap)
{
    QApplication::clipboard()->setImage(pixmap->toImage());
}
