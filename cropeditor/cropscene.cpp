#include "cropscene.hpp"
#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QTimer>

CropScene::CropScene(QObject *parent) : QGraphicsScene(parent), prevButtons(Qt::NoButton)
{
    QTimer::singleShot(0, [&] {
        QPolygonF poly;
        poly.append(sceneRect().topLeft());
        poly.append(sceneRect().topRight());
        poly.append(sceneRect().bottomRight());
        poly.append(sceneRect().bottomLeft());
        polyItem = new QGraphicsPolygonItem(poly);
        polyItem->setBrush(QBrush(QColor(0, 0, 0, 191)));
        polyItem->setPen(QPen(Qt::NoPen));
        addItem(polyItem);
    });
}

void CropScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    auto buttons = e->buttons();
    if (buttons == Qt::LeftButton || prevButtons == Qt::NoButton)
    {
        QPointF p = e->scenePos();
        if (rect == nullptr)
        {
            rect = new QGraphicsRectItem(p.x(), p.y(), 1, 1);
            initPos = p;
            QPen pen(Qt::NoBrush, 1);
            pen.setColor(Qt::cyan);
            rect->setPen(pen);
            addItem(rect);
        }
        else
        {
            if (prevButtons == Qt::NoButton)
            {
                initPos = p;
                rect->setRect(p.x(), p.y(), 1, 1);
            }
            else
            {
                rect->setRect(QRect(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()),
                                    qAbs(initPos.y() - p.y())));
            }
        }
        QPolygonF poly;
        QPointF theMagicWikipediaPoint(rect->rect().right(), sceneRect().bottom());
        poly << sceneRect().topLeft();
        poly << sceneRect().topRight();
        poly << sceneRect().bottomRight();
        poly << theMagicWikipediaPoint;
        poly << rect->rect().bottomRight();
        poly << rect->rect().topRight();
        poly << rect->rect().topLeft();
        poly << rect->rect().bottomLeft();
        poly << rect->rect().bottomRight();
        poly << theMagicWikipediaPoint;
        poly << sceneRect().bottomLeft();
        poly << sceneRect().topLeft();

        this->polyItem->setPolygon(poly);
        e->accept();
    }
    else
        QGraphicsScene::mouseMoveEvent(e);
    prevButtons = buttons;
}

void CropScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    prevButtons = Qt::NoButton;
    QGraphicsScene::mouseReleaseEvent(e);
}

void CropScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) done();
}

void CropScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    done();
    QGraphicsScene::mouseDoubleClickEvent(e);
}

void CropScene::done()
{
    for (QGraphicsView *v : views()) v->close();
    emit closedWithRect(rect != nullptr ? rect->rect().toRect() : QRect());
}
