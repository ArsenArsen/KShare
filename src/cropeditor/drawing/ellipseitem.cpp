#include "ellipseitem.hpp"

void EllipseItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (!ellie) {
        ellie = scene->addEllipse(scene->cursorPos().x(), scene->cursorPos().y(), 0, 0, scene->pen(), scene->brush());
        initPos = scene->cursorPos();
    } else {
        auto p = scene->cursorPos();
        ellie->setRect(QRectF(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()),
                              qAbs(initPos.y() - p.y())));
    }
}
