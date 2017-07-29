#ifndef COLORPICKERSCENE_HPP
#define COLORPICKERSCENE_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <screenshotutil.hpp>

class ColorPickerScene : public QGraphicsScene, public QGraphicsView {
    Q_OBJECT
public:
    ColorPickerScene(QPixmap pixmap, QWidget *parentWidget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    static void showPicker() {
        new ColorPickerScene(screenshotutil::fullscreen(), 0);
    }

private:
    QImage image;
    QColor color;
    QGraphicsEllipseItem *ellipse = 0;
    QGraphicsPixmapItem *pItem = 0;
    QGraphicsTextItem *text;
    QGraphicsRectItem *textBackground;
};

#endif // COLORPICKERSCENE_HPP
