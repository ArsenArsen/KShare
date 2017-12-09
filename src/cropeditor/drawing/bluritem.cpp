#include "bluritem.hpp"

#include <settings.hpp>

bool BlurItem::init(CropScene *) {
    effect = new QGraphicsBlurEffect;
    QFlags<QGraphicsBlurEffect::BlurHint> blurHints;
    blurHints.setFlag(QGraphicsBlurEffect::AnimationHint, settings::settings().value("blur/animatedHint", false).toBool());
    blurHints.setFlag(QGraphicsBlurEffect::PerformanceHint, settings::settings().value("blur/performanceHint", true).toBool());
    blurHints.setFlag(QGraphicsBlurEffect::QualityHint, settings::settings().value("blur/qualityHint", false).toBool());
    effect->setBlurHints(blurHints);
    effect->setBlurRadius(settings::settings().value("blur/radius", 5.).toDouble());
    return true;
}

void BlurItem::mouseDragEvent(QGraphicsSceneMouseEvent *, CropScene *scene) {
    if (pos.isNull()) {
        pos = scene->cursorPos();
        rect = scene->addRect(QRect(scene->cursorPos().toPoint(), QSize(1, 1)), QPen(scene->highlight()), Qt::NoBrush);
        pixmap = scene->addPixmap(scene->pixmap().copy(rect->rect().toRect()));
        pixmap->setPos(scene->cursorPos());
        pixmap->setGraphicsEffect(effect);
    } else {
        QPointF p = scene->cursorPos();
        rect->setRect(QRect(qMin(pos.x(), p.x()), qMin(pos.y(), p.y()), qAbs(pos.x() - p.x()), qAbs(pos.y() - p.y())));
        auto area = rect->rect();
        if (area.width() > 1 && area.height() > 1 && area.top() > 1 && area.left() > 1)
            pixmap->setPixmap(scene->pixmap().copy(rect->rect().toRect()));
        pixmap->setPos(rect->rect().topLeft());
    }
}

void BlurItem::mouseDragEndEvent(QGraphicsSceneMouseEvent *, CropScene *) {
    delete rect;
    rect = 0;
}
