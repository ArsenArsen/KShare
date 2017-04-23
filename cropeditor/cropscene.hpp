#ifndef CROPSCENE_HPP
#define CROPSCENE_HPP

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class CropScene : public QGraphicsScene
{
    Q_OBJECT
    public:
    CropScene(QObject *parent);
    virtual ~CropScene();
    signals:
    void closedWithRect(QRect rect);

    protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;

    void keyReleaseEvent(QKeyEvent *e) override;

    private:
    void done();
    QFlags<Qt::MouseButton> prevButtons;
    QGraphicsRectItem *rect = nullptr;
    QPointF initPos;
};

#endif // CROPSCENE_HPP
