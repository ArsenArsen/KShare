#ifndef ERASERITEM_HPP
#define ERASERITEM_HPP

#include "drawitem.hpp"

class EraserItem : public DrawItem {
public:
    EraserItem();
    ~EraserItem();
    QString name() override {
        return "Eraser";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override {
    }
};

#endif // ERASERITEM_HPP
