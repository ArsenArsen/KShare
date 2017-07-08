#include "lineitem.hpp"

LineItem::LineItem() {
}

void LineItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (init.isNull()) {
        init = scene->cursorPosition();
        line = scene->addLine(QLineF(init, init), scene->pen());
    } else {
        line->setLine(QLineF(init, scene->cursorPosition()));
    }
}

void LineItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
}
