#include "pathitem.hpp"

PathItem::PathItem()
{
}

PathItem::~PathItem()
{
    delete path;
}

void PathItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene)
{
    if (path == nullptr)
    {
        path = new QPainterPath(e->scenePos());
        pathItem = scene->addPath(*path, scene->pen(), scene->brush());
    }
    else
    {
        path->quadTo(path->currentPosition(), e->scenePos());
        pathItem->setPath(*path);
    }
}

void PathItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *)
{
}
