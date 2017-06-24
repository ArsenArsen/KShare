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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowFlags(windowFlags() | Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QTimer::singleShot(0, [&] {
        adjustSize();
        move(0, 0);
        if (rect().intersects(recordingArea)) // Formatter please
            move(QApplication::primaryScreen()->size().width() - rect().width(), 0);
        if (rect().intersects(recordingArea)) // Formatter please
            move(0, QApplication::primaryScreen()->size().height() - rect().height());
        if (rect().intersects(recordingArea))
            move(QApplication::primaryScreen()->size().width() - rect().width(),
                 QApplication::primaryScreen()->size().height() - rect().height());
        if (!rect().intersects(recordingArea)) show();
    });
    label = new QLabel;
    hintLabel = new QLabel;
    auto ly = new QGridLayout(this);
    setLayout(ly);
    auto size = recordingArea.size().scaled(QSize(300, 300), Qt::KeepAspectRatio);
    label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    label->setMaximumSize(size);
    label->resize(size);
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
    label->setPixmap(map.scaled(label->maximumSize()));
}
void RecordingPreview::setTime(QString time, int frame) {
    if (isVisible())
        hintLabel->setText(QString("Time: ") + time + "\nFrame: " + QString::number(frame)
                           + "\nStop key: " + hotkeying::sequence("recordingstop"));
}
