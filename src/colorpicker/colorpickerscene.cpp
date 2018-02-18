#include "colorpickerscene.hpp"
#include <QApplication>
#include <QClipboard>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <logger.hpp>
#include <screenoverlay/screenoverlayview.hpp>
#include <settings.hpp>

ColorPickerScene::ColorPickerScene(QPixmap pixmap, QWidget *parentWidget)
: ScreenOverlay(pixmap, parentWidget), ScreenOverlayView(this, parentWidget), image(pixmap.toImage()) {
    setWindowTitle(tr("KShare Color Picker"));
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::BlankCursor);

    activateWindow();
    setGeometry(pixmap.rect());
    ScreenOverlay::show();
}

void ColorPickerScene::keyPressEvent(QKeyEvent *event) {
    color = image.pixelColor(cursorPos().toPoint());
    if (event->key() == Qt::Key_Return) {
        QApplication::clipboard()->setText(color.name());
        logger::info(tr("Copied hex code to clipboard."));
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape) close();
}

void ColorPickerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    color = image.pixelColor(cursorPos().toPoint());
    QApplication::clipboard()->setText(color.name());
    close();
    logger::info(tr("Copied hex code to clipboard."));
}

QString ColorPickerScene::generateHint() {
    color = image.pixelColor(cursorPos().toPoint());
    return color.name();
}
