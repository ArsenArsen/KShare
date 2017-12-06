#include "colorpickerscene.hpp"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
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

void ColorPickerScene::mouseMoved(QGraphicsSceneMouseEvent *, QPointF cursorPos, QPointF) {
    color = image.pixelColor(cursorPos.toPoint());
}

void ColorPickerScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        QApplication::clipboard()->setText(color.name());
        qInfo().noquote() << tr("Copied hex code to clipboard.");
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape) close();
}

void ColorPickerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    QApplication::clipboard()->setText(color.name());
    close();
    qInfo().noquote() << tr("Copied hex code to clipboard.");
}

QString ColorPickerScene::generateHint() {
    return color.name();
}
