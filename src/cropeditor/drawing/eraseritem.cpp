#include "eraseritem.hpp"

EraserItem::EraserItem() {
}

EraserItem::~EraserItem() {
}

void EraserItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    for (auto i : scene->items()) {
        if (i->contains(scene->cursorPos()) && i->zValue() != -1 && i != scene->polyItm() && i != scene->selRect()) {
            scene->removeItem(i);
            break;
        }
    }
}
