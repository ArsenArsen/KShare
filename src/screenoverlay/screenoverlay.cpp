#include "screenoverlay.hpp"

#include "screenoverlaysettings.hpp"
#include <QApplication>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMenu>
#include <settings.hpp>
#include <utils.hpp>

ScreenOverlay::ScreenOverlay(QPixmap pixmap, QObject *parent) : QGraphicsScene(parent), _pixmap(pixmap) {
    setCursorPos(QCursor::pos());
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
    loadSettings();
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
        setCursorPos(cursorPos() + (delta / 2));
        QCursor::setPos(views()[0]->mapToGlobal(cursorPos().toPoint()));
    } else
        setCursorPos(e->scenePos());
    updateMag();
    mouseMoved(e, cursorPos(), delta);
}

void ScreenOverlay::moveMouse(QPoint newPos, bool spaceHeld) {
    QMouseEvent eve(QEvent::MouseMove, newPos, spaceHeld ? Qt::LeftButton : Qt::NoButton,
                    spaceHeld ? Qt::LeftButton : Qt::NoButton, Qt::NoModifier);
    for (auto &v : views()) {
        QCursor::setPos(v->mapToGlobal(newPos));
        QApplication::sendEvent(v->viewport(), &eve);
    }
}

void ScreenOverlay::moveMouseBy(QPoint delta, bool spaceHeld) {
    moveMouse(cursorPos().toPoint() + delta, spaceHeld);
}

void ScreenOverlay::hideMag() {
    setMagVisibility(false);
}

void ScreenOverlay::setMagVisibility(bool visible) {
    magnifier->setVisible(visible);
    cursorItem->setVisible(visible);
    magnifierBox->setVisible(visible);
    magnifierHint->setVisible(visible);
    magnifierHintBox->setVisible(visible);
    for (auto *view : views()) {
        view->setCursor(visible ? Qt::BlankCursor : Qt::ArrowCursor);
    }
}

void ScreenOverlay::showMag() {
    setMagVisibility(true);
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
    cursorItem->setPos(cursorPos());
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
    updateMag();
    highlightChanged(highlight);
}

void ScreenOverlay::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Space) {
        selectActive = !selectActive;
        if (!selectActive) {
            for (auto *v : views()) {
                QMouseEvent eve(QEvent::MouseButtonRelease, cursorPos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
                QApplication::sendEvent(v->viewport(), &eve);
            }
        }
    }
    switch (movementPattern()) {
    case MP_JKL:
        if (e->key() == Qt::Key_J)
            moveMouseBy(QPoint(-1, 0), selectActive);
        else if (e->key() == Qt::Key_K)
            moveMouseBy(QPoint(0, 1), selectActive);
        else if (e->key() == Qt::Key_L)
            moveMouseBy(QPoint(0, -1), selectActive);
        else if (e->key() == Qt::Key_Semicolon)
            moveMouseBy(QPoint(1, 0), selectActive);
        break;
    case MP_HJKL:
        if (e->key() == Qt::Key_H)
            moveMouseBy(QPoint(-1, 0), selectActive);
        else if (e->key() == Qt::Key_J)
            moveMouseBy(QPoint(0, 1), selectActive);
        else if (e->key() == Qt::Key_K)
            moveMouseBy(QPoint(0, -1), selectActive);
        else if (e->key() == Qt::Key_L)
            moveMouseBy(QPoint(1, 0), selectActive);
        break;
    case MP_ARROWS:
        if (e->key() == Qt::Key_Left)
            moveMouseBy(QPoint(-1, 0), selectActive);
        else if (e->key() == Qt::Key_Down)
            moveMouseBy(QPoint(0, 1), selectActive);
        else if (e->key() == Qt::Key_Up)
            moveMouseBy(QPoint(0, -1), selectActive);
        else if (e->key() == Qt::Key_Right)
            moveMouseBy(QPoint(1, 0), selectActive);
        break;
    }
    e->accept();
}

void ScreenOverlay::hide() {
    for (auto &v : views()) {
        v->hide();
    }
}

void ScreenOverlay::show() {
    for (auto &v : views()) {
        if (QApplication::screens().size() > 1)
            v->show();
        else
            v->showFullScreen();
    }
}

void ScreenOverlay::showSettings() {
    hide();
    ScreenOverlaySettings(this).exec();
    show();
}

void ScreenOverlay::loadSettings() {
    setHighlight(settings::settings().value("highlightColor", QColor(Qt::cyan)).value<QColor>());
    setMovementPattern(settings::settings().value("movementPattern", MP_HJKL).value<MovementPattern>());
    setGrid(settings::settings().value("gridEnabled", true).toBool());
}

void ScreenOverlay::contextMenuEvent(QGraphicsSceneContextMenuEvent *e) {
    QMenu menu(e->widget());
    connect(menu.addAction("Screen overlay settings"), &QAction::triggered, this, &ScreenOverlay::showSettings);
    customizeContextMenu(e, &menu);
    menu.exec(e->screenPos());
}

void ScreenOverlay::setCursorPos(QPointF cursorPos) {
    if (!pixmap().rect().contains(cursorPos.toPoint())) return;
    _cursorPos = cursorPos;
}

QPointF ScreenOverlay::cursorPos() {
    return _cursorPos;
}

void ScreenOverlay::setGrid(bool grid) {
    _grid = grid;
    if (grid) {
        updateMagnifierGrid();
    } else {
        for (auto r : gridRectsX) delete r;
        gridRectsX.clear();
        for (auto r : gridRectsY) delete r;
        gridRectsY.clear();
    }
}

bool ScreenOverlay::grid() {
    return _grid;
}

QColor ScreenOverlay::highlight() {
    return _highlight;
}

QPixmap &ScreenOverlay::pixmap() {
    return _pixmap;
}

void ScreenOverlay::setMovementPattern(MovementPattern nmp) {
    _movementPattern = nmp;
}

ScreenOverlay::MovementPattern ScreenOverlay::movementPattern() {
    return _movementPattern;
}

bool ScreenOverlay::keyboardActiveSelection() {
    return selectActive;
}
