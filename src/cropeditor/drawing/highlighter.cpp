#include "highlighter.hpp"

#include <QImage>

void Highlighter::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (pos.isNull()) {
        pos = scene->cursorPos();
        rect = scene->addRect(QRect(scene->cursorPos().toPoint(), QSize(1, 1)), QPen(scene->highlight()), Qt::NoBrush);
        pixmap = scene->addPixmap(scene->pixmap().copy(rect->rect().toRect()));
        pixmap->setPos(scene->cursorPos());
    } else {
        QPointF p = scene->cursorPos();
        rect->setRect(QRect(qMin(pos.x(), p.x()), qMin(pos.y(), p.y()), qAbs(pos.x() - p.x()), qAbs(pos.y() - p.y())));
        auto area = rect->rect();
        if (area.width() > 1 && area.height() > 1 && area.top() > 1 && area.left() > 1) {
            // toImage conversion needed for access to the underlying image data for altering
            auto pm = scene->pixmap().copy(rect->rect().toRect()).toImage();
            if (pm.format() != QImage::Format_RGB32) {
                pm = pm.convertToFormat(QImage::Format_RGB32);
            }
            for (int y = 0; y < pm.height(); y++) {
                QRgb *line = (QRgb *)pm.scanLine(y);
                for (int x = 0; x < pm.width(); x++) {
                    QRgb &pix = line[x];
                    pix = qRgb(qMin(255, qRed(pix)), qMin(255, qGreen(pix)), 0);
                }
            }
            pixmap->setPixmap(QPixmap::fromImage(pm));
        }
        pixmap->setPos(rect->rect().topLeft());
    }
}

void Highlighter::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
    delete rect;
    rect = 0;
}
