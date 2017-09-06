#include "cropview.hpp"

CropView::CropView(QGraphicsScene *scene) : ScreenOverlayView(scene) {
    setCursor(Qt::BlankCursor);
}

CropView::~CropView() {
}
