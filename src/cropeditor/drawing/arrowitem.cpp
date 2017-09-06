#include "arrowitem.hpp"
#include <QtMath>
#include <settings.hpp>

void ArrowItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (init.isNull()) {
        init = scene->cursorPosition();
        line = scene->addLine(QLineF(init, init), scene->pen());
        QPainterPath poly;
        qreal w = settings::settings().value("arrow/width", 20).toDouble() / 2;
        qreal h = settings::settings().value("arrow/height", 10).toDouble();
        poly.lineTo(QPoint(-w, h));
        poly.lineTo(QPoint(0, 0));
        poly.lineTo(QPoint(w, h));
        head = scene->addPath(poly, scene->pen(), scene->brush());
    } else {
        line->setLine(QLineF(init, scene->cursorPosition()));
        head->setRotation(
        270 + qRadiansToDegrees(qAtan2((init.y() - scene->cursorPosition().y()), (init.x() - scene->cursorPosition().x()))));
    }
    head->setPos(scene->cursorPosition());
}
