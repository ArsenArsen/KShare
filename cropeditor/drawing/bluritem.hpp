#ifndef BLURITEM_HPP
#define BLURITEM_HPP

#include "drawitem.hpp"

#include <QGraphicsEffect>

class BlurItem : public DrawItem
{
    public:
    QString name()
    {
        return "Blur";
    }
    ~BlurItem()
    {
    }

    void init(CropScene *) override;
    void mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

    private:
    QGraphicsBlurEffect *effect;
    QPointF pos;
    QGraphicsRectItem *rect;
    QGraphicsPixmapItem *pixmap;
};

#endif // BLURITEM_HPP
