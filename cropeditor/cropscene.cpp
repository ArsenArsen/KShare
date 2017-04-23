#include "cropscene.hpp"
#include <QDebug>
#include <QGraphicsView>

CropScene::CropScene(QObject *parent) : QGraphicsScene(parent), prevButtons(Qt::NoButton)
{
}

CropScene::~CropScene()
{
    delete rect;
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
                rect->setRect(QRect(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()),
                                    qAbs(initPos.x() - p.x()), qAbs(initPos.y() - p.y())));
            }
        }
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
