#include "cropeditor.hpp"

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTimer>

CropEditor::CropEditor(QPixmap *image, QObject *parent) : QObject(parent)
{
    scene = new CropScene(parent, image);
    view = new CropView(scene);

    pixmapItem = new QGraphicsPixmapItem(*image);
    pixmapItem->setZValue(-1);
    scene->addItem(pixmapItem);
    scene->setSceneRect(image->rect());

    QTimer::singleShot(0, [&] { view->showFullScreen(); });

    connect(scene, &CropScene::closedWithRect, this, &CropEditor::crop);
}

CropEditor::~CropEditor()
{
    delete scene;
}

void CropEditor::crop(QRect rect)
{
    QPixmap map = view->grab(rect);
    QPixmap *cropp = new QPixmap;
    map.swap(*cropp);
    delete view;
    emit cropped(cropp);
}
