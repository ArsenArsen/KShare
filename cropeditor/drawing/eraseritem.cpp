#include "eraseritem.hpp"

EraserItem::EraserItem() {
}

EraserItem::~EraserItem() {
}

void EraserItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    for (auto i : scene->items()) {
        if (i->contains(e->scenePos()) && i->zValue() != -1 && i != scene->polyItm() && i != scene->selRect()) {
            scene->removeItem(i);
            break;
        }
    }
}
