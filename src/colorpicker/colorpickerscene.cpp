#include "colorpickerscene.hpp"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <screenoverlayview.hpp>
#include <settings.hpp>

ColorPickerScene::ColorPickerScene(QPixmap pixmap, QWidget *parentWidget)
: ScreenOverlay(pixmap, parentWidget), ScreenOverlayView(this, parentWidget), image(pixmap.toImage()) {
    setWindowTitle(tr("KShare Color Picker"));
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::BlankCursor);

    activateWindow();
    setGeometry(pixmap.rect());
    QPoint p = utils::smallestScreenCoordinate()
               + QPoint(settings::settings().value("cropx", 0).toInt(), settings::settings().value("cropy", 0).toInt());
    move(p.x(), p.y());
    if (QApplication::screens().size() == 1)
        showFullScreen();
    else
        show();
}

void ColorPickerScene::mouseMoved(QGraphicsSceneMouseEvent *event, QPointF cursor, QPointF delta) {
    color = image.pixelColor(cursorPos().toPoint());
    qreal bottom = rect().bottom(); // max y
    qreal right = rect().right();   // max x

    QPointF origPoint = cursorPos() + QPoint(25, 0);
    QPointF scopePoint = cursorPos();
    QPointF resPoint = origPoint;
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
