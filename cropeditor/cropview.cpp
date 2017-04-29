#include "cropview.hpp"

CropView::CropView(QGraphicsScene *scene) : QGraphicsView(scene)
{
    setFrameShape(QFrame::NoFrame); // Time taken to solve: A george99g and 38 minutes.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
}

void CropView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
        e->accept();
        return;
    }
    QGraphicsView::keyPressEvent(e);
}
