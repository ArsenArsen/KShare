#include "recordingpreview.hpp"
#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLayout>
#include <QScreen>
#include <QTimer>
#include <hotkeying.hpp>

RecordingPreview::RecordingPreview(QRect area, QWidget *parent) : QWidget(parent) {
    recordingArea = area;
    setStyleSheet("background-color: rgba(0, 0, 0, 0.7);");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowFlags(windowFlags() | Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    QTimer::singleShot(0, [&] {
        adjustSize();
        move(0, 0);
        if (geometry().intersects(recordingArea)) // Formatter please
            move(QApplication::primaryScreen()->size().width() - rect().width(), 0);
        if (geometry().intersects(recordingArea)) // Formatter please
            move(0, QApplication::primaryScreen()->size().height() - rect().height());
        if (geometry().intersects(recordingArea))
            move(QApplication::primaryScreen()->size().width() - rect().width(),
                 QApplication::primaryScreen()->size().height() - rect().height());
        if (!geometry().intersects(recordingArea)) show();
    });
    label = new QLabel;
    hintLabel = new QLabel;
    auto ly = new QGridLayout(this);
    setLayout(ly);
    layout()->addWidget(hintLabel);
    layout()->addWidget(label);
    hintLabel->setText(QString("Time: 00:00\nFrame: 0\nStop key: ") + hotkeying::sequence("recordingstop"));
}

RecordingPreview::~RecordingPreview() {
    // delete everything;
    delete label;
    delete hintLabel;
}

void RecordingPreview::setPixmap(QPixmap map) {
    label->setPixmap(map);
}
void RecordingPreview::setTime(QString time, int frame) {
    hintLabel->setText(QString("Time: ") + time + "\nFrame: " + QString::number(frame)
                       + "\nStop key: " + hotkeying::sequence("recordingstop"));
}
