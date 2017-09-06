#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "drawitem.hpp"
#include <QApplication>
#include <QGraphicsSimpleTextItem>

class TextItem : public DrawItem {
    Q_DECLARE_TR_FUNCTIONS(TextItem)
public:
    QString name() override;
    bool init(CropScene *s) override;
    void mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) override;
    void mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) override;

private:
    QGraphicsSimpleTextItem *textItem = nullptr;
    QString text;
};

#endif // TEXTITEM_HPP
