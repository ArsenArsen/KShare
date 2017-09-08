#ifndef SELECTIONRECTANGLE_HPP
#define SELECTIONRECTANGLE_HPP

#include <QGraphicsRectItem>
#include <QPainter>

class SelectionRectangle : public QGraphicsRectItem {
public:
    SelectionRectangle();
    explicit SelectionRectangle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    explicit SelectionRectangle(QRectF rect, QGraphicsItem *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget) override;
};

#endif // SELECTIONRECTANGLE_HPP
