#ifndef CROPEDITOR_HPP
#define CROPEDITOR_HPP

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QPixmap>

class CropEditor : public QObject {
    Q_OBJECT
public:
    CropEditor(QPixmap image, QObject *parent = 0);
    ~CropEditor();
signals:
    QPixmap *cropped(QPixmap pixmap);

private:
    void crop(QRect rect);
    CropScene *scene = nullptr;
    CropView *view = nullptr;
};

#endif // CROPEDITOR_HPP
