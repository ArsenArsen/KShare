#include "cropscene.hpp"
#include "selectionrectangle.hpp"
#include <QApplication>
#include <QColorDialog>
#include <QDebug>
#include <QFontDialog>
#include <QGraphicsPolygonItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QScreen>
#include <QTimer>
#include <cropeditor/drawing/arrowitem.hpp>
#include <cropeditor/drawing/bluritem.hpp>
#include <cropeditor/drawing/ellipseitem.hpp>
#include <cropeditor/drawing/eraseritem.hpp>
#include <cropeditor/drawing/lineitem.hpp>
#include <cropeditor/drawing/pathitem.hpp>
#include <cropeditor/drawing/rectitem.hpp>
#include <cropeditor/drawing/textitem.hpp>
#include <cropeditor/settings/brushpenselection.hpp>
#include <functional>
#include <settings.hpp>

CropScene::CropScene(QObject *parent, QPixmap pixmap)
: QGraphicsScene(parent), cursorPos(0, 0), drawingSelectionMaker([] { return nullptr; }), prevButtons(Qt::NoButton),
  _brush(Qt::SolidPattern), _font(settings::settings().value("font", QFont()).value<QFont>()) {
    _pixmap = pixmap;
    pen().setColor(settings::settings().value("penColor", pen().color()).value<QColor>());
    pen().setCosmetic(settings::settings().value("penCosmetic", pen().isCosmetic()).toBool());
    pen().setWidthF(settings::settings().value("penWidth", pen().widthF()).toReal());
    brush().setColor(settings::settings().value("brushColor", brush().color()).value<QColor>());
    brush().setStyle(
    static_cast<Qt::BrushStyle>(settings::settings().value("brushStyle", static_cast<int>(Qt::SolidPattern)).toInt()));
    _highlight = settings::settings().value("highlightColor", QColor(Qt::cyan)).value<QColor>();

    menu = new QMenuBar;
    addDrawingAction(menu, tr("Free draw"), ":/icons/pencil.png", [] { return new PathItem; });
    addDrawingAction(menu, tr("Blur"), ":/icons/blur.png", [] { return new BlurItem; });
    addDrawingAction(menu, tr("Straight line"), ":/icons/line.png", [] { return new LineItem; });
    addDrawingAction(menu, tr("Text"), ":/icons/text.png", [] { return new TextItem; });
    addDrawingAction(menu, tr("Rectangle"), ":/icons/rectangle.png", [] { return new RectItem; });
    addDrawingAction(menu, tr("Ellipse"), ":/icons/circle.png", [] { return new EllipseItem; });
    addDrawingAction(menu, tr("Arrow"), ":/icons/arrow.png", [] { return new ArrowItem; });

    menu->addSeparator();
    addDrawingAction(menu, tr("Eraser"), ":/icons/erase.png", [] { return new EraserItem; });
    QAction *clear = menu->addAction("");
    clear->setToolTip(tr("Clear all drawing"));
    clear->setIcon(QIcon(":/icons/delete.png"));
    connect(clear, &QAction::triggered, [&] {
        auto its = items();
        for (auto i : its) {
            if (i != polyItem && i != rect && i != cursorItem && i->zValue() != -1 && i != proxyMenu && i != hint
                && i != magnifier && i != magnifierBox && i != magnifierHint && i != magnifierHintBox
                && gridRectsX.contains(dynamic_cast<QGraphicsRectItem *>(i))
                && gridRectsY.contains(dynamic_cast<QGraphicsRectItem *>(i))) {
                removeItem(i);
            }
        }
    });

    addDrawingAction(menu, tr("None"), ":/icons/crop.png", [] { return nullptr; });

    menu->addSeparator();
    QAction *settings = menu->addAction("");
    settings->setToolTip(tr("Settings"));
    settings->setIcon(QIcon(":/icons/settings.png"));
    connect(settings, &QAction::triggered, [&] {
        hide();
        BrushPenSelection(this).exec();
        show();
    });
    menu->addSeparator();
    display = menu->addAction(drawingName);
    display->setDisabled(true);

    QAction *fonts = menu->addAction("");
    fonts->setIcon(QIcon(":/icons/fontsettings.png"));
    connect(fonts, &QAction::triggered, this, &CropScene::fontAsk);

    menu->addAction(fonts);
    menu->addSeparator();
    QAction *confirm = menu->addAction("");
    confirm->setToolTip(tr("Confirm"));
    confirm->setIcon(QIcon(":/icons/accept.png"));
    connect(confirm, &QAction::triggered, [this] { done(true); });
    menu->addAction(confirm);

    QAction *cancel = menu->addAction("");
    cancel->setToolTip(tr("Cancel"));
    cancel->setIcon(QIcon(":/icons/cancel.png"));
    connect(cancel, &QAction::triggered, [this] { done(false); });
    menu->addAction(cancel);

    QPolygonF cursorPoly;
    cursorPoly << QPoint(-10, 0) //
               << QPoint(10, 0)  //
               << QPoint(0, 0)   //
               << QPoint(0, 10)  //
               << QPoint(0, -10) //
               << QPoint(0, 0);

    cursorItem = addPolygon(cursorPoly, QPen(Qt::white));
    cursorItem->setZValue(3);

    magnifier = addPixmap(QPixmap(110, 110));
    magnifierBox = addRect(magnifier->boundingRect(), QPen(_highlight));
    magnifier->setZValue(3);
    magnifierBox->setZValue(1.1);
    magnifierBox->setParentItem(magnifier);
    magnifierHint = addText("ptr: (0, 0)\nsel: (-1, -1, 0, 0)");
    magnifierHint->setParentItem(magnifier);
    magnifierHint->setY(magnifier->boundingRect().height());
    QColor c(_highlight);
    c.setAlphaF(.25);
    magnifierHintBox = addRect(magnifierHint->boundingRect(), Qt::NoPen, c);
    magnifierHintBox->setParentItem(magnifierHint);
    magnifierHintBox->setZValue(1);
    magnifierHint->setZValue(1.1);
    updateMag();

    addItem(hint);
    hint->setPos(5, 5);
    hint->setZValue(2);
    hint->setVisible(settings::settings().value("crophint", true).toBool());

    QPolygonF poly;
    QRect prect = pixmap.rect();
    poly.append(prect.topLeft());
    poly.append(prect.topRight());
    poly.append(prect.bottomRight());
    poly.append(prect.bottomLeft());
    polyItem = new QGraphicsPolygonItem(poly);
    polyItem->setBrush(QBrush(QColor(0, 0, 0, 191)));
    polyItem->setPen(QPen(Qt::NoPen));
    polyItem->setZValue(1);
    addItem(polyItem);

    auto widget = addWidget(menu);
    widget->setFlag(QGraphicsItem::ItemIsMovable, true);
    widget->setZValue(100);
    widget->setPos(100, 100);
    proxyMenu = widget;

    QTimer::singleShot(0, [&, widget] {
        auto pf = views()[0]->mapFromGlobal(QCursor::pos());
        cursorPos = QPoint(pf.x(), pf.y());
        cursorItem->setPos(cursorPos);
        updateMag();
        auto screen = QApplication::primaryScreen();
        int w = screen->geometry().width();
        widget->setPos(views()[0]->mapToScene(
        QPoint(screen->geometry().x() + (w - widget->boundingRect().width()) / 2, screen->geometry().y() + 100)));
        setGrid(settings::settings().value("gridEnabled", true).toBool());
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

void CropScene::setHighlight(QColor highlight) {
    _highlight = highlight;
    QColor c = highlight;
    c.setAlphaF(.4);
    magnifierHintBox->setBrush(c);
    if (grid()) setGrid(true);
    if (rect) rect->setPen(highlight);
    int i = settings::settings().value("magnifierPixelCount", 11).toInt() / 2;
    if (gridRectsX.isEmpty() || gridRectsY.isEmpty()) return;
    gridRectsX[i]->setBrush(c);
    gridRectsY[i]->setBrush(c);
}

void CropScene::setDrawingSelection(QString name, std::function<DrawItem *()> drawAction) {
    if (drawingSelection) {
        delete drawingSelection;
        drawingSelection = 0;
    }
    this->setFocus();
    drawingSelectionMaker = drawAction;
    drawingSelection = drawAction();
    drawingName = name;
    display->setText(drawingName);
    if (drawingSelection)
        if (!drawingSelection->init(this)) setDrawingSelection(tr("None"), [] { return nullptr; });
    menu->adjustSize();
        auto screen = QApplication::primaryScreen();
        int w = screen->geometry().width();
        proxyMenu->setPos(views()[0]->mapToScene(
        QPoint(screen->geometry().x() + (w - proxyMenu->boundingRect().width()) / 2, screen->geometry().y() + 100)));
}

QGraphicsItem *CropScene::whichItem(QPointF scenePos) {
    for (auto item : items()) {
        if (item->sceneBoundingRect().contains(scenePos))
            if (item != polyItem && item != rect && item != cursorItem && item->zValue() != -1) return item;
    }
    return nullptr;
}

void CropScene::hide() {
    setVisible(false);
}

void CropScene::show() {
    setVisible(true);
}

void CropScene::setVisible(bool visible) {
    for (auto view : views()) {
        view->setVisible(visible);
        if (visible) {
            if (QApplication::screens().size() == 1) view->showFullScreen();
            QPoint p = screenshotutil::smallestScreenCoordinate() + QPoint(settings::settings().value("cropx", 0).toInt(),
                                                                           settings::settings().value("cropy", 0).toInt());
            view->move(p.x(), p.y());
            view->setWindowTitle(tr("KShare Crop Editor"));
            view->activateWindow();
        }
    }
}

void CropScene::fontAsk() {
    hide();
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this->font(), this->views()[0], "Font to use");
    if (ok) _font = font;
    show();
}

void CropScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    QPointF delta = e->scenePos() - cursorPos;
    if (e->modifiers() & Qt::ShiftModifier) {
        cursorPos += delta / 2;
        QCursor::setPos(views()[0]->mapToGlobal(cursorPos.toPoint()));
    } else
        cursorPos = e->scenePos();
    hint->setVisible(settings::settings().value("crophint").toBool() && !hint->sceneBoundingRect().contains(cursorPos));
    cursorItem->setPos(cursorPos);
    updateMag();

    if (rect && !drawingRect) {
        // qAbs(e->scenePos().<axis>() - rect->rect().<edge>()) < 10
        bool close = false;
        QRectF newRect = rect->rect();
        if (qAbs(e->scenePos().x() - rect->rect().right()) < 10) {
            if (qAbs(e->scenePos().y() - rect->rect().bottom()) < 10) {
                close = true;
                views()[0]->setCursor(Qt::SizeFDiagCursor);
                if (e->buttons() & Qt::LeftButton) newRect.setBottomRight(cursorPos);
            } else if (qAbs(e->scenePos().y() - rect->rect().top()) < 10) {
                close = true;
                views()[0]->setCursor(Qt::SizeBDiagCursor);
                if (e->buttons() & Qt::LeftButton) newRect.setTopRight(cursorPos);
            }
        } else if (qAbs(e->scenePos().x() - rect->rect().left()) < 10) {
            if (qAbs(e->scenePos().y() - rect->rect().top()) < 10) {
                close = true;
                views()[0]->setCursor(Qt::SizeFDiagCursor);
                if (e->buttons() & Qt::LeftButton) newRect.setTopLeft(cursorPos);
            } else if (qAbs(e->scenePos().y() - rect->rect().bottom()) < 10) {
                close = true;
                views()[0]->setCursor(Qt::SizeBDiagCursor);
                if (e->buttons() & Qt::LeftButton) newRect.setBottomLeft(cursorPos);
            }
        }
        if (!close)
            views()[0]->setCursor(Qt::BlankCursor);
        else {
            rect->setRect(newRect);
            prevButtons = e->buttons();
            updatePoly();
            return;
        }
    }

    auto buttons = e->buttons();
    if (e->modifiers() & Qt::ControlModifier && buttons == Qt::LeftButton) {
        auto item = whichItem(cursorPos);
        if (item) item->moveBy(delta.x(), delta.y());
        return;
    }
    if (buttons == Qt::LeftButton) {
        if (drawingSelection) {
            drawingSelection->mouseDragEvent(e, this);
        } else {
            QPointF p = cursorPos;
            if (rect == nullptr) {
                drawingRect = true;
                rect = new SelectionRectangle(p.x(), p.y(), 1, 1);
                initPos = p;
                QPen pen(Qt::NoBrush, 1);
                pen.setColor(_highlight);
                rect->setPen(pen);
                rect->setZValue(1);
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
            updatePoly();
            e->accept();
        }
    }
    prevButtons = buttons;

    QGraphicsScene::mouseMoveEvent(e);
}

void CropScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
    drawingRect = false;
    if (drawingSelection) {
        drawingSelection->mouseDragEndEvent(e, this);
        setDrawingSelection(drawingName, drawingSelectionMaker);
    } else if (settings::settings().value("quickMode", false).toBool())
        done(true);
    prevButtons = Qt::NoButton;

    if (!(e->modifiers() & Qt::ControlModifier)) QGraphicsScene::mouseReleaseEvent(e);
}

void CropScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    if (e->modifiers() & Qt::AltModifier) {
        auto item = whichItem(cursorItem->scenePos());
        if (item && item != proxyMenu) removeItem(item);
    }

    if (!(e->modifiers() & Qt::ControlModifier)) QGraphicsScene::mousePressEvent(e);
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

    if (grid()) initMagnifierGrid();
    updateMag();

    if (!(event->modifiers() & Qt::ControlModifier)) QGraphicsScene::wheelEvent(event);
}

void CropScene::addDrawingAction(QMenuBar *menu, QString name, QString icon, std::function<DrawItem *()> item) {
    QAction *action = menu->addAction("");
    action->setToolTip(name);
    action->setIcon(QIcon(icon));
    connect(action, &QAction::triggered, [this, menu, action, item, name](bool) { setDrawingSelection(name, item); });
}

void CropScene::keyReleaseEvent(QKeyEvent *event) {
    if (((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && !drawingSelection) || event->key() == Qt::Key_Escape)
        done(event->key() != Qt::Key_Escape);
    else if (event->key() == Qt::Key_F1) {
        bool enabled = !settings::settings().value("crophint", true).toBool();
        hint->setVisible(enabled);
        settings::settings().setValue("crophint", enabled);
    }

    if (!(event->modifiers() & Qt::ControlModifier)) QGraphicsScene::keyReleaseEvent(event);
}

void CropScene::updateMag() {
    QString rectStr("(-1, -1, 0, 0)");
    if (rect) {
        rectStr = "(%0, %1, %2, %3)";
        rectStr = rectStr.arg(rect->rect().x()).arg(rect->rect().y()).arg(rect->rect().width()).arg(rect->rect().height());
    }
    magnifierHint->setPlainText(QString("ptr: (%0, %1)\nsel: %2").arg(qRound(cursorPos.x())).arg(qRound(cursorPos.y())).arg(rectStr));
    magnifierHintBox->setRect(magnifierHint->boundingRect());

    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    QPointF magnifierTopLeft = cursorPos - QPointF(pixCnt / 2., pixCnt / 2.);
    QPointF magnifierPos = cursorPos + QPointF(5, 5);

    magnifier->setPos(magnifierPos);
    magnifier->setPixmap(_pixmap.copy(magnifierTopLeft.x(), magnifierTopLeft.y(), pixCnt, pixCnt).scaled(110, 110));
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

void CropScene::updatePoly() {
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
}

void CropScene::initMagnifierGrid() {
    if (!gridRectsX.isEmpty() || !gridRectsY.isEmpty()) return;

    QColor c(_highlight);
    c.setAlphaF(.25);
    int pixCnt = settings::settings().value("magnifierPixelCount", 11).toInt();
    if (pixCnt % 2 == 0) pixCnt++;
    for (int i = 0; i < pixCnt; i++) {
        auto gridRectX = addRect(0, i * 110. / pixCnt, 110, 110. / pixCnt, QPen(Qt::black, 0.5));
        auto gridRectY = addRect(i * 110. / pixCnt, 0, 110. / pixCnt, 110, QPen(Qt::black, 0.5));
        gridRectX->setParentItem(magnifierBox);
        gridRectY->setParentItem(magnifierBox);
        gridRectX->setZValue(5);
        gridRectY->setZValue(5);
        gridRectsX.append(gridRectX);
        gridRectsY.append(gridRectY);
        if (i == (pixCnt / 2)) {
            gridRectX->setBrush(c);
            gridRectY->setBrush(c);
        }
    }
}

void CropScene::done(bool notEsc) {
    if (notEsc && rect) {
        QRectF rect2 = rect->rect();
        hint->setVisible(false);
        rect->setRect(QRect(-100, -100, 0, 0));
        magnifier->setVisible(false);
        proxyMenu->setVisible(false);
        cursorItem->setVisible(false);
        magnifierBox->setVisible(false);
        magnifierHint->setVisible(false);
        magnifierHintBox->setVisible(false);
        emit closedWithRect(rect2.toRect());
    } else
        emit closedWithRect(QRect());
}
