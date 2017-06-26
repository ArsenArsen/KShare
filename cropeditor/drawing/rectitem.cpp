#include "rectitem.hpp"

RectItem::RectItem() {
}

RectItem::~RectItem() {
}

void RectItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (!rect) {
        rect = scene->addRect(e->scenePos().x(), e->scenePos().y(), 0, 0);
        rect->setBrush(scene->brush());
        rect->setPen(scene->pen());
        initPos = e->scenePos();
    } else {
        auto p = e->scenePos();
        rect->setRect(
        QRect(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()), qAbs(initPos.y() - p.y())));
    }
}

void RectItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
}
