#ifndef CROPVIEW_HPP
#define CROPVIEW_HPP

#include <QGraphicsView>
#include <QKeyEvent>

class CropView : public QGraphicsView {
public:
    CropView(QGraphicsScene *scene);
};

#endif // CROPVIEW_HPP
