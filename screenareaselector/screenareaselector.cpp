#include "screenareaselector.hpp"
#include <QCloseEvent>
#include <QPalette>
#include <QTimer>
#include <settings.hpp>

ScreenAreaSelector::ScreenAreaSelector() {
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background:transparent;");
    setWindowTitle("KShare: Select Area (By resizig this window)");
    QTimer::singleShot(0, [&] {
        QVariant val = settings::settings().value("screenareaselector/rect");
        if (val.canConvert<QRect>()) {
            QRect rect = val.value<QRect>();
            resize(rect.size());
            move(rect.topLeft());
        }
    });
}

ScreenAreaSelector::~ScreenAreaSelector() {
}

void ScreenAreaSelector::keyPressEvent(QKeyEvent *event) {
    event->accept();
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QRect r = QRect(pos(), rect().size());
        emit selectedArea(r);
        close();
    } else if (event->key() == Qt::Key_Escape)
        close();
}

void ScreenAreaSelector::closeEvent(QCloseEvent *) {
    QRect r = QRect(pos(), rect().size());
    settings::settings().setValue("screenareaselector/rect", r);
}
