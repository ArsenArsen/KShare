#include "cropeditor.hpp"

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTimer>
#include <settings.hpp>

CropEditor::CropEditor(QPixmap *image, QObject *parent) : QObject(parent)
{
    scene = new CropScene(parent, image);
    view = new CropView(scene);
    QPixmap *scaled = new QPixmap();
    image->scaled(view->width(), view->height()).swap(*scaled);
    pixmapItem = new QGraphicsPixmapItem(*image);
    pixmapItem->setZValue(-1);
    scene->addItem(pixmapItem);
    scene->setSceneRect(image->rect());
    view->setGeometry(0, 0, image->width(), image->height());
    view->showFullScreen();

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
