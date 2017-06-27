#include "cropscene.hpp"
#include <QColorDialog>
#include <QDebug>
#include <QFontDialog>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QTimer>
#include <cropeditor/drawing/bluritem.hpp>
#include <cropeditor/drawing/dotitem.hpp>
#include <cropeditor/drawing/ellipseitem.hpp>
#include <cropeditor/drawing/eraseritem.hpp>
#include <cropeditor/drawing/lineitem.hpp>
#include <cropeditor/drawing/pathitem.hpp>
#include <cropeditor/drawing/rectitem.hpp>
#include <cropeditor/drawing/textitem.hpp>
#include <cropeditor/settings/brushpenselection.hpp>
#include <functional>
#include <settings.hpp>

CropScene::CropScene(QObject *parent, QPixmap *pixmap)
: QGraphicsScene(parent), drawingSelectionMaker([] { return nullptr; }), prevButtons(Qt::NoButton),
  _font(settings::settings().value("font", QFont()).value<QFont>()) {
    _pixmap = pixmap;
    pen().setColor(settings::settings().value("penColor", pen().color()).value<QColor>());
    pen().setCosmetic(settings::settings().value("penCosmetic", pen().isCosmetic()).toBool());
    pen().setWidthF(settings::settings().value("penWidth", pen().widthF()).toFloat());
    brush().setColor(settings::settings().value("brushColor", brush().color()).value<QColor>());

    addDrawingAction(menu, "Dot", [] { return new DotItem; });
    addDrawingAction(menu, "Path", [] { return new PathItem; });
    addDrawingAction(menu, "Blur", [] { return new BlurItem; });
    addDrawingAction(menu, "Straight line", [] { return new LineItem; });
    addDrawingAction(menu, "Text", [] { return new TextItem; });
    addDrawingAction(menu, "Rectangle", [] { return new RectItem; });
    addDrawingAction(menu, "Ellipse", [] { return new EllipseItem; });

    menu.addSeparator();
    addDrawingAction(menu, "Eraser", [] { return new EraserItem; });
    QAction *clear = menu.addAction("Clear all drawing");
    connect(clear, &QAction::triggered, [&] {
        auto its = items();
        for (auto i : its) {
            if (i != rect && i != polyItem && i->zValue() != -1) removeItem(i);
        }
    });

    QAction *reset = menu.addAction("Reset pen selection");
    connect(reset, &QAction::triggered, [&] { setDrawingSelection("None", [] { return nullptr; }); });

    menu.addSeparator();
    QAction *settings = new QAction;
    settings->setText("Settings");
    menu.addSeparator();
    display = menu.addAction(drawingName);
    display->setDisabled(true);
    connect(settings, &QAction::triggered, [&] { BrushPenSelection(this).exec(); });
    menu.addAction(settings);

    magnifier = addPixmap(pixmap->copy(0, 0, 11, 11).scaled(110, 110));
    magnifierBox = addRect(magnifier->boundingRect(), QPen(Qt::cyan));
    magnifier->setZValue(1);
    magnifierBox->setZValue(1.1);
    magnifierBox->setParentItem(magnifier);
    magnifierHint = addText("ptr: (0, 0)\nsel: (0, 0, 0, 0)");
    magnifierHint->setParentItem(magnifier);
    magnifierHint->setY(magnifier->boundingRect().height());
    QColor c(Qt::cyan);
    c.setAlphaF(.25);
    magnifierHintBox = addRect(magnifierHint->boundingRect(), Qt::NoPen, c);
    magnifierHintBox->setParentItem(magnifierHint);
    magnifierHintBox->setZValue(1);
    magnifierHint->setZValue(1.1);
    initMagnifierGrid();
    updateMag(QPointF(0, 0));

    connect(menu.addAction("Set Font"), &QAction::triggered, this, &CropScene::fontAsk);

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

CropScene::~CropScene() {
    delete drawingSelection;
}

QPen &CropScene::pen() {
    return _pen;
}

QBrush &CropScene::brush() {
    return _brush;
}

QFont &CropScene::font() {
    return _font;
}

void CropScene::setDrawingSelection(QString name, std::function<DrawItem *()> drawAction) {
    this->setFocus();
    drawingSelectionMaker = drawAction;
    drawingSelection = drawAction();
    drawingName = name;
    if (drawingSelection)
        if (!drawingSelection->init(this)) setDrawingSelection("None", [] { return nullptr; });
}

void CropScene::fontAsk() {
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this->font(), nullptr, "Font to use");
    if (ok) _font = font;
}

void CropScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    updateMag(e->scenePos());

    auto buttons = e->buttons();
    if (e->modifiers() & Qt::ControlModifier && buttons == Qt::LeftButton) {
        QTransform stupidThing = views()[0]->transform();
        auto item = itemAt(e->screenPos(), stupidThing);
        if (item && item != polyItem && item != rect && item->zValue() != -1) {
            QPointF delta = e->scenePos() - e->lastScenePos();
            item->moveBy(delta.x(), delta.y());
        }
        return;
    }
    if (buttons == Qt::LeftButton || (prevButtons == Qt::NoButton && prevButtons != buttons)) {
        if (drawingSelection) {
            drawingSelection->mouseDragEvent(e, this);
        } else {
            QPointF p = e->scenePos();
            if (rect == nullptr) {
                rect = new QGraphicsRectItem(p.x(), p.y(), 1, 1);
                initPos = p;
                QPen pen(Qt::NoBrush, 1);
                pen.setColor(Qt::cyan);
                rect->setPen(pen);
                addItem(rect);
            } else {
                if (prevButtons == Qt::NoButton) {
                    initPos = p;
                    rect->setRect(p.x(), p.y(), 1, 1);
                } else {
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

void CropScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
    if (drawingSelection) {
        drawingSelection->mouseDragEndEvent(e, this);
        delete drawingSelection;
        drawingSelection = drawingSelectionMaker();
        if (drawingSelection)
            if (!drawingSelection->init(this)) setDrawingSelection("None", [] { return nullptr; });
    } else if (settings::settings().value("quickMode", false).toBool())
        done();
    prevButtons = Qt::NoButton;
}

void CropScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    if (pixCnt > 20) return;
    if (event->delta() > 0 && pixCnt < 19)
        settings::settings().setValue("magnifierPixelCount", pixCnt += 2);
    else if (pixCnt > 1)
        settings::settings().setValue("magnifierPixelCount", pixCnt -= 2);

    for (auto item : gridRectsX) delete item;
    gridRectsX.clear();
    for (auto item : gridRectsY) delete item;
    gridRectsY.clear();

    initMagnifierGrid();
    updateMag(event->scenePos());
}

void CropScene::addDrawingAction(QMenu &menu, QString name, std::function<DrawItem *()> item) {
    QAction *action = new QAction;
    action->setText(name);
    connect(action, &QAction::triggered, [this, item, name](bool) { setDrawingSelection(name, item); });
    menu.addAction(action);
}

void CropScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *e) {
    display->setText(drawingName);
    menu.exec(e->screenPos());
    e->accept();
}

void CropScene::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) done(); // Segfault
}

void CropScene::updateMag(QPointF scenePos) {
    QString rectStr("(0, 0, 0, 0)");
    if (rect) {
        rectStr = "(%0, %1, %2, %3)";
        rectStr = rectStr.arg(rect->rect().x()).arg(rect->rect().y()).arg(rect->rect().width()).arg(rect->rect().height());
    }
    magnifierHint->setPlainText(QString("ptr: (%0, %1)\nsel: %2").arg(scenePos.x()).arg(scenePos.y()).arg(rectStr));
    magnifierHintBox->setRect(magnifierHint->boundingRect());

    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    QPointF magnifierTopLeft = scenePos - QPointF(pixCnt / 2., pixCnt / 2.);
    QPointF magnifierPos = scenePos + QPointF(5, 5);

    magnifier->setPos(magnifierPos);
    magnifier->setPixmap(_pixmap->copy(magnifierTopLeft.x(), magnifierTopLeft.y(), pixCnt, pixCnt).scaled(110, 110));
    QPointF bottomRight = magnifierHintBox->sceneBoundingRect().bottomRight();
    if (magnifier->sceneBoundingRect().bottom() > bottomRight.y())
        bottomRight.setY(magnifier->sceneBoundingRect().bottom());

    if (magnifier->sceneBoundingRect().right() > bottomRight.x())
        bottomRight.setX(magnifier->sceneBoundingRect().right());

    if (bottomRight.x() > sceneRect().right())
        magnifierPos -= QPointF(qMax(130., magnifierHintBox->boundingRect().width()), 0);
    if (bottomRight.y() > sceneRect().bottom())
        magnifierPos -= QPointF(0, 130 + magnifierHintBox->boundingRect().height());
    magnifier->setPos(magnifierPos);
}

void CropScene::initMagnifierGrid() {
    QColor c(Qt::cyan);
    c.setAlphaF(.25);
    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    for (int i = 0; i < pixCnt; i++) {
        auto gridRectX = addRect(0, i * 110. / pixCnt, 110, 110. / pixCnt, QPen(Qt::black));
        auto gridRectY = addRect(i * 110. / pixCnt, 0, 110. / pixCnt, 110, QPen(Qt::black));
        gridRectX->setParentItem(magnifierBox);
        gridRectY->setParentItem(magnifierBox);
        gridRectX->setZValue(1);
        gridRectY->setZValue(1);
        gridRectsX.append(gridRectX);
        gridRectsY.append(gridRectY);
        if (i == (pixCnt / 2)) {
            gridRectX->setBrush(c);
            gridRectY->setBrush(c);
        }
    }
}

void CropScene::done() {
    if (rect) {
        rect->setPen(QPen(Qt::NoPen));
        magnifier->setVisible(false);
        magnifierBox->setVisible(false);
        magnifierHint->setVisible(false);
        magnifierHintBox->setVisible(false);
        emit closedWithRect(rect->rect().toRect());
    }
}
