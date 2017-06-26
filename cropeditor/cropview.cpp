#include "cropview.hpp"

CropView::CropView(QGraphicsScene *scene) : QGraphicsView(scene) {
    setFrameShape(QFrame::NoFrame); // Time taken to solve: A george99g and 38 minutes.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    setCursor(QCursor(Qt::CrossCursor));
    setMouseTracking(true);
}

void CropView::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        close();
        e->accept();
        return;
    }
    QGraphicsView::keyPressEvent(e);
}
