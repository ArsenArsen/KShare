#include "arrowitem.hpp"
#include <QtMath>
#include <settings.hpp>

void ArrowItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (init.isNull()) {
        init = e->scenePos();
        line = scene->addLine(QLineF(init, init), scene->pen());
        QPainterPath poly;
        qreal w = settings::settings().value("arrow/width", 20).toDouble() / 2;
        qreal h = settings::settings().value("arrow/height", 10).toDouble();
        poly.lineTo(QPoint(-w, h));
        poly.lineTo(QPoint(0, 0));
        poly.lineTo(QPoint(w, h));
        head = scene->addPath(poly, scene->pen(), scene->brush());
    } else {
        line->setLine(QLineF(init, e->scenePos()));
        head->setRotation(270 + qRadiansToDegrees(qAtan2((init.y() - e->scenePos().y()), (init.x() - e->scenePos().x()))));
    }
    head->setPos(e->scenePos());
}
