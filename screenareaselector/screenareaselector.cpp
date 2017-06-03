#include "screenareaselector.hpp"
#include <QCloseEvent>
#include <QLabel>
#include <QPalette>
#include <QStackedLayout>
#include <QTimer>
#include <settings.hpp>

ScreenAreaSelector::ScreenAreaSelector() {
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background-color: rgba(0, 0, 0, 0.5);");
    setWindowTitle("KShare: Select Area (By resizing this window)");
    QTimer::singleShot(0, [&] {
        QVariant val = settings::settings().value("screenareaselector/rect");
        if (val.canConvert<QRect>()) {
            QRect rect = val.value<QRect>();
            resize(rect.size());
            move(rect.topLeft());
        }
        hintLabel->setText(QString::number(width()) + "x" + QString::number(height()));
        show();
    });
    setLayout(new QStackedLayout());
    hintLabel = new QLabel();
    hintLabel->setAlignment(Qt::AlignCenter);
    layout()->addWidget(hintLabel);
}

ScreenAreaSelector::~ScreenAreaSelector() {
    delete hintLabel;
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

void ScreenAreaSelector::resizeEvent(QResizeEvent *) {
    hintLabel->setText(QString::number(width()) + "x" + QString::number(height()));
}

void ScreenAreaSelector::closeEvent(QCloseEvent *) {
    QRect r = QRect(pos(), rect().size());
    settings::settings().setValue("screenareaselector/rect", r);
}
