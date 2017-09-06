#ifndef PATHITEM_HPP
#define PATHITEM_HPP

#include "../cropscene.hpp"
#include "drawitem.hpp"

class PathItem : public DrawItem {
public:
    PathItem();
    ~PathItem();
    QString name() {
        return "Path";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene);
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *e, CropScene *scene);

private:
    QPainterPath *path = nullptr;
    QGraphicsPathItem *pathItem = nullptr;
};

#endif // PATHITEM_HPP
