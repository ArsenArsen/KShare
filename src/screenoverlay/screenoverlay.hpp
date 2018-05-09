#ifndef SCREENOVERLAY_HPP
#define SCREENOVERLAY_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMetaType>

class ScreenOverlay : public QGraphicsScene {
    Q_OBJECT
public:
    enum MovementPattern { MP_JKL, MP_HJKL, MP_ARROWS };

    explicit ScreenOverlay(QPixmap pixmap, QObject *parent = 0);

    MovementPattern movementPattern();
    void setMovementPattern(MovementPattern nmp);

    QPixmap &pixmap();
    void updateMagnifierGrid();
    QColor highlight();
    void setHighlight(QColor highlight);
    QColor foreground();
    void setForeground(QColor foreground);
    bool grid();
    void setGrid(bool grid);
    QPointF cursorPos();
    void setCursorPos(QPointF cursorPos);

public slots:
    void showSettings();
    void hide();
    void show();
    void loadSettings();
    void updateMag();
    void hideMag();
    void showMag();
    void setMagVisibility(bool visible);
    void moveMouse(QPoint newPoint, bool spaceHeld = false);
    void moveMouseBy(QPoint delta, bool spaceHeld = false);

protected:
    bool keyboardActiveSelection();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void wheelEvent(QGraphicsSceneWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) override;

    virtual void mouseMoved(QGraphicsSceneMouseEvent *, QPointF, QPointF) {
    }
    virtual void highlightChanged(QColor) {
    }
    virtual QString generateHint() {
        return QString();
    }
    virtual void customizeContextMenu(QGraphicsSceneContextMenuEvent *, QMenu *) {
    }

private:
    QPointF _cursorPos = QPoint(0, 0);
    QGraphicsPixmapItem *magnifier = nullptr;
    QGraphicsRectItem *magnifierBox = nullptr;
    QGraphicsTextItem *magnifierHint = nullptr;
    QGraphicsRectItem *magnifierHintBox = nullptr;
    QGraphicsPolygonItem *cursorItem = nullptr;
    QList<QGraphicsRectItem *> gridRectsX;
    QList<QGraphicsRectItem *> gridRectsY;
    QColor _highlight = Qt::cyan, _foreground = Qt::white;
    bool _grid = true;
    bool selectActive = false;
    QPixmap _pixmap;
    MovementPattern _movementPattern = MP_ARROWS;
};

Q_DECLARE_METATYPE(ScreenOverlay::MovementPattern)

#endif /* SCREENOVERLAY_HPP */
