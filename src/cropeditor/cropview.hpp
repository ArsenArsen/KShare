#ifndef CROPVIEW_HPP
#define CROPVIEW_HPP

#include <QKeyEvent>
#include <QCoreApplication>
#include <screenoverlay/screenoverlayview.hpp>

class CropView : public ScreenOverlayView {
    Q_DECLARE_TR_FUNCTIONS(CropScene)
public:
    CropView(QGraphicsScene *scene);
    ~CropView();
protected:
    void showEvent(QShowEvent *e) override;
};

#endif // CROPVIEW_HPP
