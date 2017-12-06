#ifndef COLORPICKERSCENE_HPP
#define COLORPICKERSCENE_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <screenoverlayview.hpp>
#include <screenoverlay.hpp>
#include <utils.hpp>

class ColorPickerScene : public ScreenOverlay, public ScreenOverlayView {
    Q_DECLARE_TR_FUNCTIONS(ColorPickerScene)
public:
    ColorPickerScene(QPixmap pixmap, QWidget *parent = nullptr);
    void mouseMoved(QGraphicsSceneMouseEvent *event, QPointF cursorPos, QPointF delta) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    QString generateHint() override;
    static void showPicker() {
        new ColorPickerScene(utils::fullscreen());
    }

private:
    QImage image;
    QColor color;
};

#endif // COLORPICKERSCENE_HPP
