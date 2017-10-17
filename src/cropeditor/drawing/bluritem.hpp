#ifndef BLURITEM_HPP
#define BLURITEM_HPP

#include "drawitem.hpp"

#include <QGraphicsEffect>

class BlurItem : public DrawItem {
public:
    QString name() override {
        return "Blur";
    }
    ~BlurItem() {
    }

    bool init(CropScene *) override;
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

private:
    QGraphicsBlurEffect *effect;
    QPointF pos;
    QGraphicsRectItem *rect = 0;
    QGraphicsPixmapItem *pixmap;
};

#endif // BLURITEM_HPP
