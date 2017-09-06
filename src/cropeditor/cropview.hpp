#ifndef CROPVIEW_HPP
#define CROPVIEW_HPP

#include <QKeyEvent>
#include <screenoverlayview.hpp>

class CropView : public ScreenOverlayView {
public:
    CropView(QGraphicsScene *scene);
    ~CropView();
};

#endif // CROPVIEW_HPP
