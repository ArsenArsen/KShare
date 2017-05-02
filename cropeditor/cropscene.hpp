#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <functional>
class CropScene;

#include <cropeditor/drawing/drawitem.hpp>

class CropScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    CropScene(QObject *parent, QPixmap *pixmap);
    ~CropScene();
    QPen &pen();
    QBrush &brush();
    void setDrawingSelection(QString name, std::function<DrawItem *()> drawAction);
    QPixmap *pixmap()
    {
        return _pixmap;
    }

    signals:
    void closedWithRect(QRect rect);

    protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) override;

    void keyReleaseEvent(QKeyEvent *e) override;

    private:
    void addDrawingAction(QMenu &menu, QString name, std::function<DrawItem *()> item);
    void done();
    QPixmap *_pixmap;
    QFlags<Qt::MouseButton> prevButtons;
    QGraphicsRectItem *rect = nullptr;
    QPointF initPos;
    QPen _pen;
    QBrush _brush;
    QGraphicsPolygonItem *polyItem = nullptr;
    std::function<DrawItem *()> drawingSelectionMaker;
    DrawItem *drawingSelection = nullptr;
    QMenu menu;
    QString drawingName = "None";
    QAction *display;
};

#endif // CROPSCENE_HPP
