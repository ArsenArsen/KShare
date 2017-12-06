#include "cropview.hpp"

#include <settings.hpp>
#include <utils.hpp>

CropView::CropView(QGraphicsScene *scene) : ScreenOverlayView(scene) {
    setCursor(Qt::BlankCursor);
}

CropView::~CropView() {
}

void CropView::showEvent(QShowEvent *) {
    QPoint p = utils::smallestScreenCoordinate()
               + QPoint(settings::settings().value("cropx", 0).toInt(), settings::settings().value("cropy", 0).toInt());
    if (QApplication::screens().size() == 1) showFullScreen();
    move(p);
    setWindowTitle(tr("KShare Crop Editor"));
    activateWindow();
}
