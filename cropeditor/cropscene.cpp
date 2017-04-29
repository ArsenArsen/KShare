#include "cropscene.hpp"
#include <QColorDialog>
#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QTimer>
#include <cropeditor/drawing/dotitem.hpp>

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

QPen CropScene::pen()
{
    return _pen;
}

QBrush CropScene::brush()
{
    return _brush;
}

void CropScene::setDrawingSelection(DrawItem *drawAction)
{
    drawingSelection = drawAction;
}

void CropScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    auto buttons = e->buttons();
    if (buttons == Qt::LeftButton || prevButtons == Qt::NoButton)
    {
        if (drawingSelection != nullptr)
        {
            drawingSelection->mouseDragEvent(e, this);
        }
        else
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
    }
    prevButtons = buttons;
}

void CropScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (drawingSelection != nullptr)
    {
        drawingSelection->mouseDragEndEvent(e, this);
        drawingSelection = nullptr;
    }
    prevButtons = Qt::NoButton;
}

void CropScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) done();
}

void CropScene::addDrawingAction(QMenu &menu, DrawItem *item)
{
    QAction *action = new QAction;
    action->setText(item->name());
    QObject::connect(action, &QAction::triggered,
                     [&](bool) { QTimer::singleShot(0, [&] { setDrawingSelection(item); }); });
    menu.addAction(action);
}

void CropScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
    //    QMenu menu(e->widget());

    //    addDrawingAction(menu, new DotItem);

    //    menu.addSeparator();
    //    QAction *bColorAction = new QAction;
    //    bColorAction->setText("Select brush color");
    //    connect(bColorAction, &QAction::triggered, [&] { _brush.setColor(QColorDialog::getColor(_brush.color())); });
    //    QAction *pColorAction = new QAction;
    //    pColorAction->setText("Select pen color");
    //    connect(pColorAction, &QAction::triggered, [&] { _pen.setColor(QColorDialog::getColor(_pen.color())); });
    //    menu.addActions({ pColorAction, bColorAction });

    //    menu.exec(e->screenPos());
    e->accept();
}

void CropScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    done();
}

void CropScene::done()
{
    if (rect)
    {
        rect->setPen(QPen(Qt::NoPen));
        emit closedWithRect(rect->rect().toRect());
    }
}
