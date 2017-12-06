#include "screenoverlay.hpp"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <settings.hpp>
#include <utils.hpp>

ScreenOverlay::ScreenOverlay(QPixmap pixmap, QObject *parent) : QGraphicsScene(parent), _pixmap(pixmap) {
    addPixmap(pixmap)->setZValue(-1);
    QPolygonF cursorPoly;
    cursorPoly << QPoint(-10, 0) //
               << QPoint(10, 0)  //
               << QPoint(0, 0)   //
               << QPoint(0, 10)  //
               << QPoint(0, -10) //
               << QPoint(0, 0);

    cursorItem = addPolygon(cursorPoly, QPen(Qt::white));
    cursorItem->setZValue(199);

    magnifier = addPixmap(QPixmap(110, 110));
    magnifierBox = addRect(magnifier->boundingRect(), QPen(highlight()));
    magnifier->setZValue(199);
    magnifierBox->setZValue(199);
    magnifierBox->setParentItem(magnifier);
    magnifierHint = addText("ptr: (0, 0)\nsel: (-1, -1, 0, 0)");
    magnifierHint->setParentItem(magnifier);
    magnifierHint->setY(magnifier->boundingRect().height());
    QColor c(highlight());
    c.setAlphaF(.25);
    magnifierHintBox = addRect(magnifierHint->boundingRect(), Qt::NoPen, c);
    magnifierHintBox->setParentItem(magnifierHint);
    magnifierHintBox->setZValue(199);
    magnifierHint->setZValue(199);
    updateMag();
}

void ScreenOverlay::wheelEvent(QGraphicsSceneWheelEvent *e) {
    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    if (pixCnt > 20) return;
    if (e->delta() > 0 && pixCnt < 19)
        settings::settings().setValue("magnifierPixelCount", pixCnt += 2);
    else if (pixCnt > 1)
        settings::settings().setValue("magnifierPixelCount", pixCnt -= 2);

    for (auto item : gridRectsX) delete item;
    gridRectsX.clear();
    for (auto item : gridRectsY) delete item;
    gridRectsY.clear();

    if (grid()) updateMagnifierGrid();
    updateMag();

    QGraphicsScene::wheelEvent(e);
}

void ScreenOverlay::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    QPointF delta = e->scenePos() - cursorPos();
    if (e->modifiers() & Qt::ShiftModifier) {
        _cursorPos += delta / 2;
        QCursor::setPos(views()[0]->mapToGlobal(cursorPos().toPoint()));
    } else
        setCursorPos(e->scenePos());
    cursorItem->setPos(cursorPos());
    updateMag();
    mouseMoved(e, cursorPos(), delta);
}

void ScreenOverlay::hideMag() {
    magnifier->setVisible(false);
    cursorItem->setVisible(false);
    magnifierBox->setVisible(false);
    magnifierHint->setVisible(false);
    magnifierHintBox->setVisible(false);
}

void ScreenOverlay::updateMagnifierGrid() {
    if (!grid()) return;
    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    if (gridRectsX.size() == gridRectsY.size() && gridRectsY.size() == pixCnt) return;

    QColor c(highlight());
    c.setAlphaF(.25);
    for (int i = 0; i < pixCnt; i++) {
        auto gridRectX = addRect(0, i * 110. / pixCnt, 110, 110. / pixCnt, QPen(Qt::black, 0.5));
        auto gridRectY = addRect(i * 110. / pixCnt, 0, 110. / pixCnt, 110, QPen(Qt::black, 0.5));
        gridRectX->setParentItem(magnifierBox);
        gridRectY->setParentItem(magnifierBox);
        gridRectX->setZValue(199);
        gridRectY->setZValue(199);
        gridRectsX.append(gridRectX);
        gridRectsY.append(gridRectY);
        if (i == (pixCnt / 2)) {
            gridRectX->setBrush(c);
            gridRectY->setBrush(c);
        }
    }
}

void ScreenOverlay::updateMag() {
    updateMagnifierGrid();
    QString hint = generateHint();
    magnifierHint->setVisible(!hint.isEmpty());
    magnifierHintBox->setVisible(!hint.isEmpty());
    magnifierHint->setPlainText(hint);
    magnifierHintBox->setRect(magnifierHint->boundingRect());

    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    QPointF magnifierTopLeft = cursorPos() - QPointF(pixCnt / 2, pixCnt / 2);
    QPointF magnifierPos = cursorPos() + QPointF(5, 5);

    magnifier->setPos(magnifierPos);
    magnifier->setPixmap(utils::extend(pixmap(), pixCnt, utils::invertColor(highlight()))
                         .copy(magnifierTopLeft.x() + pixCnt, magnifierTopLeft.y() + pixCnt, pixCnt, pixCnt)
                         .scaled(110, 110));
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

void ScreenOverlay::setHighlight(QColor highlight) {
    _highlight = highlight;
    QColor c = highlight;
    c.setAlphaF(.4);
    magnifierHintBox->setBrush(c);
    magnifierBox->setPen(c);
    if (grid()) setGrid(true);
    int i = settings::settings().value("magnifierPixelCount", 11).toInt() / 2;
    if (gridRectsX.isEmpty() || gridRectsY.isEmpty()) return;
    gridRectsX[i]->setBrush(c);
    gridRectsY[i]->setBrush(c);
    highlightChanged(highlight);
}
