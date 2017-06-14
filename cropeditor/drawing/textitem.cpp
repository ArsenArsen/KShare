#include "textitem.hpp"
#include <QInputDialog>
#include <QtMath>

bool TextItem::init(CropScene *) {
    bool ok;
    text = QInputDialog::getText(nullptr, "Text to add", "Input", QLineEdit::Normal, QString(), &ok);
    return ok;
}

void TextItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (!textItem) {
        textItem = scene->addSimpleText(text, scene->font());
        textItem->setPos(e->scenePos());
        textItem->setPen(scene->pen());
        textItem->setBrush(scene->brush());
    } else {
        auto ee
        = 180 + qRadiansToDegrees(qAtan2((textItem->pos().y() - e->scenePos().y()), (textItem->pos().x() - e->scenePos().x())));
        textItem->setRotation(ee);
    }
}

void TextItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
}

QString TextItem::name() {
    return "Text";
}
