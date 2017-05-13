#include "cropeditor.hpp"

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QApplication>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QScreen>
#include <QTimer>
#include <settings.hpp>

CropEditor::CropEditor(QPixmap *image, QObject *parent) : QObject(parent) {
    scene = new CropScene(parent, image);
    view = new CropView(scene);
    qreal ratio = QApplication::primaryScreen()->devicePixelRatio();
    pixmapItem = new QGraphicsPixmapItem(image->scaled(image->width() / ratio, image->height() / ratio));
    pixmapItem->setZValue(-1);
    scene->addItem(pixmapItem);
    scene->setSceneRect(image->rect());
    view->showFullScreen();

    QTimer::singleShot(0, [&] { view->showFullScreen(); });

    connect(scene, &CropScene::closedWithRect, this, &CropEditor::crop);
}

CropEditor::~CropEditor() {
    delete scene;
}

void CropEditor::crop(QRect rect) {
    QPixmap map = view->grab(rect);
    QPixmap *cropp = new QPixmap;
    map.swap(*cropp);
    delete view;
    emit cropped(cropp);
}
