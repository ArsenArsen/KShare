#ifndef SCREENOVERLAYVIEW_HPP
#define SCREENOVERLAYVIEW_HPP

#include <QGraphicsView>

class ScreenOverlayView : public QGraphicsView {
public:
    ScreenOverlayView(QGraphicsScene *scene, QWidget *parent = 0);
    ~ScreenOverlayView() {
    }

protected:
    void showEvent(QShowEvent *);
};

#endif // SCREENOVERLAYVIEW_HPP
