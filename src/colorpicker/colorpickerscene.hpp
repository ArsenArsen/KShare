#ifndef COLORPICKERSCENE_HPP
#define COLORPICKERSCENE_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <screenoverlay/screenoverlay.hpp>
#include <screenoverlay/screenoverlayview.hpp>
#include <utils.hpp>

class ColorPickerScene : public ScreenOverlay, public ScreenOverlayView {
    Q_DECLARE_TR_FUNCTIONS(ColorPickerScene)
public:
    ColorPickerScene(QPixmap pixmap, QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    static void showPicker() {
        new ColorPickerScene(utils::fullscreen());
    }

protected:
    QString generateHint() override;

private:
    QImage image;
    QColor color;
};

#endif // COLORPICKERSCENE_HPP
