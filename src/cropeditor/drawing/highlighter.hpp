#ifndef HIGHLIGHTER_HPP
#define HIGHLIGHTER_HPP

#include "drawitem.hpp"
#include <QPointF>

class Highlighter : public DrawItem {
public:
    QString name() override {
        return "Highlighter";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

private:
    QPointF pos;
    QGraphicsRectItem *rect = 0;
    QGraphicsPixmapItem *pixmap;
};

#endif /* HIGHLIGHTER_HPP */
