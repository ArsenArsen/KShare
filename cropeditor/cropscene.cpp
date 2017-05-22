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
#include <cropeditor/drawing/lineitem.hpp>
#include <cropeditor/drawing/pathitem.hpp>
#include <cropeditor/drawing/textitem.hpp>
#include <cropeditor/settings/brushpenselection.hpp>
#include <functional>
#include <settings.hpp>

CropScene::CropScene(QObject *parent, QPixmap *pixmap)
: QGraphicsScene(parent), drawingSelectionMaker([] { return nullptr; }), prevButtons(Qt::NoButton),
  _font(settings::settings().value("font", QFont()).value<QFont>()) {
    pen().setColor(settings::settings().value("penColor", pen().color()).value<QColor>());
    pen().setCosmetic(settings::settings().value("penCosmetic", pen().isCosmetic()).toBool());
    pen().setWidthF(settings::settings().value("penWidth", pen().widthF()).toFloat());
    brush().setColor(settings::settings().value("brushColor", brush().color()).value<QColor>());

    addDrawingAction(menu, "Dot", [] { return new DotItem; });
    addDrawingAction(menu, "Path", [] { return new PathItem; });
    addDrawingAction(menu, "Blur", [] { return new BlurItem; });
    addDrawingAction(menu, "Straight line", [] { return new LineItem; });
    addDrawingAction(menu, "Text", [] { return new TextItem; });

    QAction *reset = menu.addAction("Reset");
    connect(reset, &QAction::triggered, [&] { setDrawingSelection("None", [] { return nullptr; }); });

    menu.addSeparator();
    QAction *settings = new QAction;
    settings->setText("Settings");
    menu.addSeparator();
    display = menu.addAction(drawingName);
    display->setDisabled(true);
    connect(settings, &QAction::triggered, [&] { BrushPenSelection(this).exec(); });
    menu.addAction(settings);

    connect(menu.addAction("Set Font"), &QAction::triggered, this, &CropScene::fontAsk);

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
    auto buttons = e->buttons();
    if (buttons == Qt::LeftButton || prevButtons == Qt::NoButton) {
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
                    rect->setRect(QRect(qMin(initPos.x(), p.x()), qMin(initPos.y(), p.y()), qAbs(initPos.x() - p.x()), qAbs(initPos.y() - p.y())));
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
    qDebug() << "release";
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

void CropScene::done() {
    if (rect) {
        rect->setPen(QPen(Qt::NoPen));
        emit closedWithRect(rect->rect().toRect());
    }
}
