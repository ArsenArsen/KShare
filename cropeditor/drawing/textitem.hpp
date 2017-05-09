#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "drawitem.hpp"
#include <QGraphicsSimpleTextItem>

class TextItem : public DrawItem {
    public:
    QString name() override;
    bool init(CropScene *) override;
    void mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

    private:
    QGraphicsSimpleTextItem *textItem = nullptr;
    QString text;
};

#endif // TEXTITEM_HPP
