#ifndef COLORPICKERSCENE_HPP
#define COLORPICKERSCENE_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <screenoverlayview.hpp>
#include <screenshotutil.hpp>

class ColorPickerScene : public QGraphicsScene, public ScreenOverlayView {
    Q_DECLARE_TR_FUNCTIONS(ColorPickerScene)
public:
    ColorPickerScene(QPixmap pixmap, QWidget *parent = nullptr);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    static void showPicker() {
        new ColorPickerScene(screenshotutil::fullscreen());
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
