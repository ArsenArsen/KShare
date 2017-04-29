#include "lineitem.hpp"

LineItem::LineItem()
{
}

LineItem::~LineItem()
{
    delete path;
}

void LineItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *)
{
    if (path == nullptr)
        path = new QPainterPath(e->scenePos());
    else
        path->lineTo(e->scenePos());
}

void LineItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *scene)
{
    scene->addPath(*path, scene->pen(), scene->brush());
}
