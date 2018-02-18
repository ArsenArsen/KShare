#include "recordingpreview.hpp"
#include <QApplication>
#include <QGridLayout>
#include <QLayout>
#include <QScreen>
#include <QTimer>
#include <hotkeying.hpp>

static QSize max(300, 300);

inline bool sizeGreater(QSize one, QSize two) {
    return one.height() > two.height() || one.width() > two.width();
}

RecordingPreview::RecordingPreview(QRect area, QWidget *parent) : QWidget(parent) {
    recordingArea = area;
    setStyleSheet("background-color: rgba(0, 0, 0, 0.7);");
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowFlags(windowFlags() | Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QTimer::singleShot(0, [&] {
        adjustSize();
        move(0, 0);
        if (frameGeometry().intersects(recordingArea)) // Formatter please
            move(QApplication::primaryScreen()->size().width() - rect().width(), 0);
        if (frameGeometry().intersects(recordingArea)) // Formatter please
            move(0, QApplication::primaryScreen()->size().height() - rect().height());
        if (frameGeometry().intersects(recordingArea))
            move(QApplication::primaryScreen()->size().width() - rect().width(),
                 QApplication::primaryScreen()->size().height() - rect().height());
        if (!frameGeometry().intersects(recordingArea)) show();
    });
    label = new QLabel;
    hintLabel = new QLabel;
    auto ly = new QGridLayout(this);
    setLayout(ly);
    size = sizeGreater(recordingArea.size(), max) ? recordingArea.size().scaled(max, Qt::KeepAspectRatio) :
                                                    recordingArea.size();
    label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    label->setMaximumSize(size);
    QPixmap empty(size);
    empty.fill(Qt::transparent);
    label->setPixmap(empty);
    layout()->addWidget(hintLabel);
    layout()->addWidget(label);
    hintLabel->setText(QString(tr("Time: 00:00\nFrame: 0\nStop key: ")) + hotkeying::sequence("recordingstop"));
}

RecordingPreview::~RecordingPreview() {
    // delete everything;
    delete label;
    delete hintLabel;
}

void RecordingPreview::setPixmap(QPixmap map) {
    label->setPixmap(map.scaled(size));
}
void RecordingPreview::setTime(QString time, int frame) {
    if (isVisible())
        hintLabel->setText(tr("Time: %1\nFrame: %2\nStop key: %3").arg(time).arg(frame).arg(hotkeying::sequence("recordingstop")));
}
