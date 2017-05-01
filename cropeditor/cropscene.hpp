#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class CropScene;

#include <cropeditor/drawing/drawitem.hpp>

class CropScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    CropScene(QObject *parent, QPixmap *pixmap);
    QPen &pen();
    QBrush &brush();
    void setDrawingSelection(DrawItem *drawAction);
    QPixmap *pixmap()
    {
        return _pixmap;
    }

    signals:
    void closedWithRect(QRect rect);

    protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) override;

    void keyReleaseEvent(QKeyEvent *e) override;

    private:
    void addDrawingAction(QMenu &menu, DrawItem *item);
    void done();
    QPixmap *_pixmap;
    QFlags<Qt::MouseButton> prevButtons;
    QGraphicsRectItem *rect = nullptr;
    QPointF initPos;
    QPen _pen;
    QBrush _brush;
    QGraphicsPolygonItem *polyItem = nullptr;
    DrawItem *drawingSelection = nullptr;
};

#endif // CROPSCENE_HPP
