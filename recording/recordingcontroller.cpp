#include "recordingcontroller.hpp"
#include <QImage>
#include <QRect>
#include <QTimer>
#include <iostream>
#include <mainwindow.hpp>
#include <screenareaselector/screenareaselector.hpp>
#include <screenshotutil.hpp>
#include <settings.hpp>
#include <stdio.h>
#include <uploaders/uploadersingleton.hpp>
#include <worker/worker.hpp>

RecordingController::RecordingController() : timer(this) {
    connect(&timer, &QTimer::timeout, this, &RecordingController::timeout);
}

bool RecordingController::isRunning() {
    return timer.isActive();
}

bool RecordingController::start(RecordingContext *context) {
    if (isRunning()) return false;
    if (_context) delete _context;
    _context = context;
    ScreenAreaSelector *sel = new ScreenAreaSelector;
    connect(sel, &ScreenAreaSelector::selectedArea, this, &RecordingController::startWithArea);
    return true;
}

bool RecordingController::end() {
    if (!isRunning()) return false;
    timer.stop();
    area = QRect();
    preview->close();
    preview = 0;
    UploaderSingleton::inst().upload(_context->finalizer());
    frame = 0;
    time = 0;
    return true;
}

void RecordingController::timeout() {
    if (isRunning()) {
        time++;
        int localTime = time * timer.interval() - 3000;
        if (localTime > 0) {
            QPixmap *pp = screenshotutil::fullscreenArea(settings::settings().value("captureCursor", true).toBool(),
                                                         area.x(), area.y(), area.width(), area.height());
            QScopedPointer<QPixmap> p(pp);
            WorkerContext *context = new WorkerContext;
            context->consumer = _context->consumer;
            context->targetFormat = _context->format;
            context->pixmap = *pp;
            frame++;
            preview->setPixmap(*pp);
            Worker::queue(context);
        }
        long second = localTime / 1000 % 60;
        long minute = localTime / 60000;
        preview->setTime(QString("%1:%2").arg(QString::number(minute)).arg(QString::number(second)), frame);
    }
}

void RecordingController::startWithArea(QRect newArea) {
    area = newArea;
    preview = new RecordingPreview(newArea);
    timer.start(1000 / settings::settings().value("recording/framerate", 30).toInt());
}
