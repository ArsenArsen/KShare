#include "cropeditor.hpp"

#include "cropscene.hpp"
#include "cropview.hpp"
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QScreen>
#include <QTimer>
#include <settings.hpp>

CropEditor::CropEditor(QPixmap image, QObject *parent) : QObject(parent) {
    scene = new CropScene(parent, image);
    view = new CropView(scene);
    qreal ratio = QApplication::primaryScreen()->devicePixelRatio();
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(image);
    pixmapItem->setZValue(-1);
    pixmapItem->setScale(1 / ratio);
    scene->addItem(pixmapItem);
    scene->setSceneRect(image.rect());
    view->resize(image.width(), image.height());
    view->setMinimumSize(image.size());
    view->move(settings::settings().value("cropx", 0).toInt(), settings::settings().value("cropy", 0).toInt());
    view->setWindowTitle("KShare Crop Editor");
    view->show();

    connect(scene, &CropScene::closedWithRect, this, &CropEditor::crop);
}

CropEditor::~CropEditor() {
    scene->deleteLater();
    view->deleteLater();
}

void CropEditor::crop(QRect rect) {
    if (rect.isValid()) emit cropped(view->grab(rect));
    deleteLater();
}
