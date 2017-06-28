#include "bluritem.hpp"

#include <settings.hpp>

bool BlurItem::init(CropScene *) {
    effect = new QGraphicsBlurEffect;
    QFlags<QGraphicsBlurEffect::BlurHint> blurHints;
    blurHints.setFlag(QGraphicsBlurEffect::AnimationHint, settings::settings().value("blur/animatedHint", false).toBool());
    blurHints.setFlag(QGraphicsBlurEffect::PerformanceHint, settings::settings().value("blur/performanceHint", true).toBool());
    blurHints.setFlag(QGraphicsBlurEffect::QualityHint, settings::settings().value("blur/qualityHint", false).toBool());
    effect->setBlurHints(blurHints);
    effect->setBlurRadius(settings::settings().value("blurRadius", 5.).toDouble());
    return true;
}

void BlurItem::mouseDragEvent(QGraphicsSceneMouseEvent *e, CropScene *scene) {
    if (pos.isNull()) {
        pos = e->scenePos();
        rect = scene->addRect(QRect(e->scenePos().toPoint(), QSize(1, 1)), QPen(Qt::cyan), Qt::NoBrush);
        pixmap = scene->addPixmap(scene->pixmap()->copy(rect->rect().toRect()));
        pixmap->setPos(e->scenePos());
        pixmap->setZValue(rect->zValue() - 0.1);
        pixmap->setGraphicsEffect(effect);
    } else {
        QPointF p = e->scenePos();
        rect->setRect(QRect(qMin(pos.x(), p.x()), qMin(pos.y(), p.y()), qAbs(pos.x() - p.x()), qAbs(pos.y() - p.y())));
        pixmap->setPixmap(scene->pixmap()->copy(rect->rect().toRect()));
        pixmap->setPos(rect->rect().topLeft());
    }
}

void BlurItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
    if (rect != nullptr) rect->setPen(Qt::NoPen);
}
