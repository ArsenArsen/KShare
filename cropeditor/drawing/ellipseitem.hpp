#ifndef ELLIPSEITEM_HPP
#define ELLIPSEITEM_HPP

#include "drawitem.hpp"

class EllipseItem : public DrawItem {
public:
    EllipseItem() {
    }
    QString name() override {
        return "Blur";
    }
    ~EllipseItem() {
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override {
    }

private:
    QGraphicsEllipseItem *ellie = nullptr;
    QPointF initPos;
};

#endif // ELLIPSEITEM_HPP
