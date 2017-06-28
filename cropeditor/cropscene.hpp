#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QFont>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <functional>
class CropScene;

#include <cropeditor/drawing/drawitem.hpp>

class CropScene : public QGraphicsScene {
    Q_OBJECT
public:
    CropScene(QObject *parent, QPixmap *pixmap);
    ~CropScene();
    QPen &pen();
    QBrush &brush();
    QFont &font();
    void setDrawingSelection(QString name, std::function<DrawItem *()> drawAction);
    QPixmap *pixmap() {
        return _pixmap;
    }
    QGraphicsPolygonItem *polyItm() {
        return polyItem;
    }
    QGraphicsRectItem *selRect() {
        return rect;
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
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) override;

    void keyReleaseEvent(QKeyEvent *e) override;

private:
    void updateMag(QPointF scenePos);
    void initMagnifierGrid();
    void addDrawingAction(QMenu &menu, QString name, std::function<DrawItem *()> item);
    void done();
    std::function<DrawItem *()> drawingSelectionMaker;
    QFlags<Qt::MouseButton> prevButtons;
    QPixmap *_pixmap;
    QGraphicsRectItem *rect = nullptr;
    QGraphicsPixmapItem *magnifier = nullptr;
    QGraphicsRectItem *magnifierBox = nullptr;
    QGraphicsTextItem *magnifierHint = nullptr;
    QGraphicsRectItem *magnifierHintBox = nullptr;
    QPointF initPos;
    QPen _pen;
    QBrush _brush;
    QFont _font;
    QGraphicsPolygonItem *polyItem = nullptr;
    DrawItem *drawingSelection = nullptr;
    QMenu menu;
    QString drawingName = "None";
    QAction *display;
    QList<QGraphicsRectItem *> gridRectsX;
    QList<QGraphicsRectItem *> gridRectsY;
};

#endif // CROPSCENE_HPP
