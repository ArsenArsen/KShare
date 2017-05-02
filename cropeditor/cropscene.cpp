#include "cropscene.hpp"
#include <QColorDialog>
#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QTimer>
#include <cropeditor/drawing/bluritem.hpp>
#include <cropeditor/drawing/dotitem.hpp>
#include <cropeditor/drawing/lineitem.hpp>
#include <cropeditor/drawing/pathitem.hpp>
#include <cropeditor/settings/brushpenselection.hpp>
#include <settings.hpp>

CropScene::CropScene(QObject *parent, QPixmap *pixmap) : QGraphicsScene(parent), prevButtons(Qt::NoButton)
{
    pen().setColor(settings::settings().value("penColor", pen().color()).value<QColor>());
    pen().setCosmetic(settings::settings().value("penCosmetic", pen().isCosmetic()).toBool());
    pen().setWidthF(settings::settings().value("penWidth", pen().widthF()).toFloat());
    brush().setColor(settings::settings().value("brushColor", brush().color()).value<QColor>());

    _pixmap = pixmap;
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

QPen &CropScene::pen()
{
    return _pen;
}

QBrush &CropScene::brush()
{
    return _brush;
}

void CropScene::setDrawingSelection(DrawItem *drawAction)
{
    drawingSelection = drawAction;
    drawAction->init(this);
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
        delete drawingSelection;
        drawingSelection = nullptr;
    }
    prevButtons = Qt::NoButton;
}

void CropScene::addDrawingAction(QMenu &menu, DrawItem *item)
{
    QAction *action = new QAction;
    action->setText(item->name());
    connect(action, &QAction::triggered, [this, item](bool) { setDrawingSelection(item); });
    menu.addAction(action);
}

void CropScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
    QMenu menu(e->widget());

    addDrawingAction(menu, new DotItem);
    addDrawingAction(menu, new PathItem);
    addDrawingAction(menu, new BlurItem);
    addDrawingAction(menu, new LineItem);

    menu.addSeparator();
    QAction *settings = new QAction;
    settings->setText("Settings");
    connect(settings, &QAction::triggered, [&] { BrushPenSelection(this).exec(); });
    menu.addAction(settings);

    menu.exec(e->screenPos());
    e->accept();
}

void CropScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) done(); // Segfault
}

void CropScene::done()
{
    if (rect)
    {
        rect->setPen(QPen(Qt::NoPen));
        emit closedWithRect(rect->rect().toRect());
    }
}
