#include "rectitem.hpp"

RectItem::RectItem() {
}

RectItem::~RectItem() {
}

void RectItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (!rect) {
        rect = scene->addRect(scene->cursorPosition().x(), scene->cursorPosition().y(), 0, 0);
        rect->setBrush(scene->brush());
        rect->setPen(scene->pen());
        initPos = scene->cursorPosition();
    } else {
        auto p = scene->cursorPosition();
        rect->setRect(
        QRect(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()), qAbs(initPos.y() - p.y())));
    }
}

void RectItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
}
