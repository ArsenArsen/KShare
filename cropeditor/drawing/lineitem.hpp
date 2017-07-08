#ifndef LINEITEM_HPP
#define LINEITEM_HPP

#include "drawitem.hpp"

class LineItem : public DrawItem {
public:
    LineItem();
    QString name() override {
        return "Straight line";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

private:
    QPointF init;
    QGraphicsLineItem *line;
};

#endif // LINEITEM_HPP
