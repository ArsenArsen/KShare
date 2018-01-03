#include "selectionrectangle.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

SelectionRectangle::SelectionRectangle() {
}

SelectionRectangle::SelectionRectangle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
: QGraphicsRectItem(x, y, w, h, parent) {
}

SelectionRectangle::SelectionRectangle(QRectF rect, QGraphicsItem *parent)
: SelectionRectangle(rect.left(), rect.top(), rect.width(), rect.height(), parent) {
}

void SelectionRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget) {
    QRect rect = this->rect().toRect();
    if (rect.height() > 30 && rect.width() > 30) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(pen().color());

        // Before you panick, the <angle> * 16 things are needed because here angles are 1/16th of their value
        // The <x/y> - 10 is accounting for sizes.
        // Trail and error ftw

        // Bottom left
        painter->drawPie(rect.left() - 10, rect.bottom() - 10, 20, 20, 0, 90 * 16);
        // Top right
        painter->drawPie(rect.right() - 10, rect.bottom() - 10, 20, 20, 90 * 16, 90 * 16);


        // Top left
        painter->drawPie(rect.left() - 10, rect.top() - 10, 20, 20, 270 * 16, 90 * 16);
        // Top right
        painter->drawPie(rect.right() - 10, rect.top() - 10, 20, 20, 180 * 16, 90 * 16);
    }
    painter->setBrush(brush());
    painter->setPen(pen());
    QGraphicsRectItem::paint(painter, options, widget);
}
