#ifndef LINEITEM_HPP
#define LINEITEM_HPP

#include "../cropscene.hpp"
#include "drawitem.hpp"

class LineItem : public DrawItem
{
    public:
    LineItem();
    ~LineItem();
    QString name()
    {
        return "Line";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene);
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *e, CropScene *scene);

    private:
    QPainterPath *path = nullptr;
};

#endif // LINEITEM_HPP
