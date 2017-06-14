#ifndef DOTITEM_HPP
#define DOTITEM_HPP

#include "../cropscene.hpp"
#include "drawitem.hpp"

class DotItem : public DrawItem {
public:
    DotItem();
    ~DotItem();
    QString name() {
        return "Dots (drag to add)";
    }
    void mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene);
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *e, CropScene *scene);
};

#endif // DOTITEM_HPP
