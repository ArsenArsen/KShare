#include "dotitem.hpp"

DotItem::DotItem()
{
}

DotItem::~DotItem()
{
}

void DotItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene)
{
    scene->addEllipse(e->pos().x() - 1.5, e->pos().y() - 1.5, 3, 3, scene->pen(), scene->brush());
}

void DotItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *)
{
}
