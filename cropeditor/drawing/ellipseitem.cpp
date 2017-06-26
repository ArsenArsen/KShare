#include "ellipseitem.hpp"

void EllipseItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (!ellie) {
        ellie = scene->addEllipse(e->scenePos().x(), e->scenePos().y(), 0, 0, scene->pen(), scene->brush());
        initPos = e->scenePos();
    } else {
        auto p = e->scenePos();
        ellie->setRect(QRectF(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()),
                              qAbs(initPos.y() - p.y())));
    }
}
