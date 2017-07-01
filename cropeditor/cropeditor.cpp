#include "cropeditor.hpp"

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QScreen>
#include <QTimer>
#include <settings.hpp>

CropEditor::CropEditor(QPixmap *image, QObject *parent) : QObject(parent) {
    scene = new CropScene(parent, image);
    view = new CropView(scene);
    qreal ratio = QApplication::primaryScreen()->devicePixelRatio();
    pixmapItem = new QGraphicsPixmapItem(*image);
    pixmapItem->setZValue(-1);
    pixmapItem->setScale(1 / ratio);
    scene->addItem(pixmapItem);
    scene->setSceneRect(image->rect());
    view->resize(image->width(), image->height());
    view->setMinimumSize(image->size());
    view->move(0, 0);
    view->show();

    connect(scene, &CropScene::closedWithRect, this, &CropEditor::crop);
}

CropEditor::~CropEditor() {
    delete scene;
    delete view;
}

void CropEditor::crop(QRect rect) {
    QPixmap map = view->grab(rect);
    QPixmap *cropp = new QPixmap;
    map.swap(*cropp);
    delete view;
    emit cropped(cropp);
}
