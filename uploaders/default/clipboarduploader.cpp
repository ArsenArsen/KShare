#include "clipboarduploader.hpp"

#include <QApplication>
#include <QClipboard>
#include <notifications.hpp>

void ClipboardUploader::doUpload(QPixmap *pixmap) {
  QApplication::clipboard()->setImage(pixmap->toImage());
  notifications::notify("KShare", "Copied to clipboard!");
}
