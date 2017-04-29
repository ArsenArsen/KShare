#include "cropeditor.hpp"

#include "cropview.hpp"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTimer>

CropEditor::CropEditor(QPixmap *image, QObject *parent) : QObject(parent)
{
    pixmap = image;
    scene = new CropScene(parent);
    view = new CropView(scene);

    pixmapItem = new QGraphicsPixmapItem(*pixmap);
    pixmapItem->setZValue(-1);
    scene->addItem(pixmapItem);
    scene->setSceneRect(pixmap->rect());

    QTimer::singleShot(0, [&] { view->showFullScreen(); });

    connect(scene, &CropScene::closedWithRect, this, &CropEditor::crop);
}

CropEditor::~CropEditor()
{
    delete scene;
    delete view;
    delete pixmapItem;
}

void CropEditor::crop(QRect rect)
{
    QPixmap crop = pixmap->copy(rect);
    QPixmap *cropp = new QPixmap;
    crop.swap(*cropp);
    emit cropped(cropp);
}
