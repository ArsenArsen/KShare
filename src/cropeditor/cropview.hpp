#ifndef CROPVIEW_HPP
#define CROPVIEW_HPP

#include <QCoreApplication>
#include <QKeyEvent>
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
