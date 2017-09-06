#ifndef ARROWITEM_HPP
#define ARROWITEM_HPP

#include "drawitem.hpp"


class ArrowItem : public DrawItem {
public:
    ArrowItem() {
    }
    QString name() override {
        return "Arrow";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override {
    }

private:
    QGraphicsLineItem *line;
    QGraphicsPathItem *head;
    QPointF init;
};

#endif // ARROWITEM_HPP
