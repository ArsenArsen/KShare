#include "pathitem.hpp"

#include <settings.hpp>

PathItem::PathItem() {
}

PathItem::~PathItem() {
    delete path;
}

void PathItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (path == nullptr) {
        path = new QPainterPath(e->scenePos());
        pathItem = scene->addPath(*path, scene->pen(),
                                  settings::settings().value("brushPath", false).toBool() ? scene->brush() : QBrush());
    } else {
        path->quadTo(path->currentPosition(), e->scenePos());
        pathItem->setPath(*path);
    }
}

void PathItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
}
