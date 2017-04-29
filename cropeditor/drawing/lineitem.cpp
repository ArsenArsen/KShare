#include "lineitem.hpp"

LineItem::LineItem()
{
}

LineItem::~LineItem()
{
    delete path;
}

void LineItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene)
{
    if (path == nullptr)
    {
        path = new QPainterPath(e->scenePos());
        pathItem = scene->addPath(*path, scene->pen(), scene->brush());
    }
    else
    {
        path->lineTo(e->scenePos());
        pathItem->setPath(*path);
    }
}

void LineItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *)
{
}
