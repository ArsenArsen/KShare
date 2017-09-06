#ifndef RECTITEM_HPP
#define RECTITEM_HPP

#include "drawitem.hpp"

class RectItem : public DrawItem {
public:
    RectItem();
    ~RectItem();
    QString name() override {
        return "Rectangle";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

private:
    QGraphicsRectItem *rect = nullptr;
    QPointF initPos;
};

#endif // RECTITEM_HPP
