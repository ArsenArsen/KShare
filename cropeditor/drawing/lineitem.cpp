#include "lineitem.hpp"

LineItem::LineItem()
{
}

void LineItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene)
{
    if (init.isNull())
    {
        init = e->scenePos();
        line = scene->addLine(QLineF(init, init), scene->pen());
    }
    else
    {
        line->setLine(QLineF(init, e->scenePos()));
    }
}

void LineItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *)
{
}
