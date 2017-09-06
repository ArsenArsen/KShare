#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QFont>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <functional>
#include <screenoverlayview.hpp>
#include <screenshotutil.hpp>
class CropScene;

#include <cropeditor/drawing/drawitem.hpp>

class CropScene : public QGraphicsScene {
    Q_OBJECT
public:
    CropScene(QObject *parent, QPixmap pixmap);
    ~CropScene();
    QPen &pen();
    QBrush &brush();
    QFont &font();
    QColor highlight() {
        return _highlight;
    }
    void setHighlight(QColor highlight);
    bool grid() {
        return _grid;
    }
    void setGrid(bool grid) {
        _grid = grid;
        if (grid) {
            initMagnifierGrid();
        } else {
            for (auto r : gridRectsX) delete r;
            gridRectsX.clear();
            for (auto r : gridRectsY) delete r;
            gridRectsY.clear();
        }
    }
    void setDrawingSelection(QString name, std::function<DrawItem *()> drawAction);
    QPixmap pixmap() {
        return _pixmap;
    }
    QGraphicsPolygonItem *polyItm() {
        return polyItem;
    }
    QGraphicsRectItem *selRect() {
        return rect;
    }
    QGraphicsItem *whichItem(QPointF scenePos);
    void hide();
    void show();
    void setVisible(bool visible);
    QPointF cursorPosition() {
        return cursorPos;
    }

public slots:
    void fontAsk();

signals:
    void closedWithRect(QRect rect);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override; // WHEEEEEEL
    void keyReleaseEvent(QKeyEvent *e) override;

private slots:
    void done(bool notEsc = true);

private:
    void updateMag();
    void initMagnifierGrid();
    void addDrawingAction(QMenuBar *menu, QString name, QString icon, std::function<DrawItem *()> item);
    QPointF cursorPos;
    std::function<DrawItem *()> drawingSelectionMaker;
    QFlags<Qt::MouseButton> prevButtons;
    QPixmap _pixmap;
    QGraphicsRectItem *rect = nullptr;
    QGraphicsPixmapItem *magnifier = nullptr;
    QGraphicsRectItem *magnifierBox = nullptr;
    QGraphicsTextItem *magnifierHint = nullptr;
    QGraphicsRectItem *magnifierHintBox = nullptr;
    QPointF initPos;
    QPen _pen;
    QBrush _brush;
    QFont _font;
    QColor _highlight;
    bool _grid;
    QGraphicsPolygonItem *polyItem = nullptr;
    DrawItem *drawingSelection = nullptr;
    QMenuBar *menu = nullptr;
    QGraphicsProxyWidget *proxyMenu = nullptr;
    QString drawingName = "None";
    QAction *display;
    QList<QGraphicsRectItem *> gridRectsX;
    QList<QGraphicsRectItem *> gridRectsY;
    QGraphicsPolygonItem *cursorItem = nullptr;
    QGraphicsPixmapItem *hint
    = new QGraphicsPixmapItem(screenshotutil::renderText(tr( //
                                                         "Press F1 to toggle this hint\n"
                                                         "\tHold Shift to slow the cursor down\n"
                                                         "\tCtrl+Drag a thing to move it around\n"
                                                         "\tAlt+Click a drawing to remove it\n"
                                                         "\tPress Return/Enter to finish\n"
                                                         "\tPress ESC to cancel\n"
                                                         "\tUse the menu bar to draw\n"
                                                         "\tNOTE: You must select 'Crop' before closing the editor\n"
                                                         "\tIf you do not it will not close."),
                                                         5,
                                                         QColor(0, 0, 0, 125),
                                                         Qt::white));
};

#endif // CROPSCENE_HPP
