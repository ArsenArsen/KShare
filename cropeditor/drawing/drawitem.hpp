#ifndef DRAWITEM_HPP
#define DRAWITEM_HPP

#include <QString>
#include <cropeditor/cropscene.hpp>

class DrawItem
{
    public:
    virtual QString name() = 0;
    virtual void render(QPixmap *pixmap) = 0;
    virtual void makeItem(CropScene *scene) = 0;
    virtual void mouseDragEvent(QGraphicsSceneMoveEvent *e, CropScene *scene) = 0;
};

#endif // DRAWITEM_HPP
