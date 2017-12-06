#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <cropeditor/selectionrectangle.hpp>
#include <functional>
#include <screenoverlay/screenoverlay.hpp>
#include <screenoverlay/screenoverlayview.hpp>
#include <utils.hpp>
class CropScene;

#include <cropeditor/drawing/drawitem.hpp>

class CropScene : public ScreenOverlay {
    Q_OBJECT
public:
    CropScene(QObject *parent, QPixmap pixmap);
    ~CropScene();
    QPen &pen();
    QBrush &brush();
    QFont &font();
    void setDrawingSelection(QString name, std::function<DrawItem *()> drawAction);
    QGraphicsPolygonItem *polyItm() {
        return polyItem;
    }
    QGraphicsRectItem *selRect() {
        return rect;
    }
    QGraphicsItem *whichItem(QPointF scenePos);
    void setVisible(bool visible);

public slots:
    void fontAsk();

signals:
    void closedWithRect(QRect rect);

protected:
    void mouseMoved(QGraphicsSceneMouseEvent *e, QPointF cursorPos, QPointF delta) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    QString generateHint() override;

private slots:
    void done(bool notEsc = true);

private:
    void updatePoly();
    void addDrawingAction(QMenuBar *menu, QString name, QString icon, std::function<DrawItem *()> item);
    std::function<DrawItem *()> drawingSelectionMaker;
    QFlags<Qt::MouseButton> prevButtons;
    QGraphicsRectItem *rect = nullptr;
    bool drawingRect = true;
    QPointF initPos;
    QPen _pen;
    QBrush _brush;
    QFont _font;
    QGraphicsPolygonItem *polyItem = nullptr;
    DrawItem *drawingSelection = nullptr;
    QMenuBar *menu = nullptr;
    QGraphicsProxyWidget *proxyMenu = nullptr;
    QString drawingName = "None";
    QAction *display;
    QGraphicsPixmapItem *hint
    = new QGraphicsPixmapItem(utils::renderText(tr( //
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
