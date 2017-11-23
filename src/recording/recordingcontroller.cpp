#include "recordingcontroller.hpp"
#include <QImage>
#include <QRect>
#include <QTimer>
#include <iostream>
#include <mainwindow.hpp>
#include <screenareaselector/screenareaselector.hpp>
#include <settings.hpp>
#include <stdio.h>
#include <uploaders/uploadersingleton.hpp>
#include <utils.hpp>
#include <worker/worker.hpp>

RecordingController::RecordingController() : timer(this) {
    connect(&timer, &QTimer::timeout, this, &RecordingController::timeout);
}

bool RecordingController::isRunning() {
    return preview;
}

bool RecordingController::start(RecordingContext *context) {
    if (isRunning()) return false;
    if (_context) delete _context;
    _context = context;
    ScreenAreaSelector *sel = new ScreenAreaSelector;
    connect(sel, &ScreenAreaSelector::selectedArea, this, &RecordingController::startWithArea);
    connect(this, &RecordingController::ended, sel, &ScreenAreaSelector::deleteLater);
    return true;
}

bool RecordingController::end() {
    emit ended();
    if (!isRunning()) return false;
    area = QRect();
    if (preview) {
        preview->close();
        preview->deleteLater();
    }

    preview = 0;
    WorkerContext *c = new WorkerContext;
    c->consumer = [&](QImage) {
        _QueueContext contx;
        contx.file = _context->finalizer();
        contx.format = _context->anotherFormat;
        contx.postUploadTask = _context->postUploadTask;
        queue(contx);
    };
    c->targetFormat = QImage::Format_Alpha8;
    c->pixmap = QPixmap(0, 0);
    Worker::queue(c);

    frame = 0;
    time = 0;
    return true;
}

bool RecordingController::abort() {
    emit ended();
    if (!isRunning()) return false;
    area = QRect();
    if (preview) {
        preview->close();
        preview->deleteLater();
    }

    preview = 0;
    WorkerContext *c = new WorkerContext;
    c->consumer = [&](QImage) {
        _context->finalizer();
        _context->postUploadTask();
    };
    c->targetFormat = QImage::Format_Alpha8;
    c->pixmap = QPixmap(0, 0);
    Worker::queue(c);

    frame = 0;
    time = 0;
    return true;
}

void RecordingController::queue(_QueueContext arr) {
    QMutexLocker l(&lock);
    uploadQueue.enqueue(arr);
}

void RecordingController::timeout() {
    if (isRunning()) {
        if (!_context->validator(area.size())) {
            if (preview) {
                preview->close();
                preview->deleteLater();
            }
            frame = 0;
            time = 0;
            preview = 0;
            area = QRect();
            timer.stop();
            return;
        }
        time++;
        int localTime = time * timer.interval() - 3000;
        if (localTime > 0) {
            QPixmap pp = utils::fullscreenArea(settings::settings().value("captureCursor", true).toBool(), area.x(),
                                               area.y(), area.width(), area.height());
            WorkerContext *context = new WorkerContext;
            context->consumer = _context->consumer;
            context->targetFormat = _context->format;
            context->pixmap = pp;
            frame++;
            preview->setPixmap(pp);
            Worker::queue(context);
        }
        long second = localTime / 1000 % 60;
        long minute = localTime / 60000;
        if (isRunning())
            preview->setTime(QString("%1:%2").arg(QString::number(minute)).arg(QString::number(second)), frame);
    } else {
        QMutexLocker l(&lock);
        if (!uploadQueue.isEmpty()) {
            auto a = uploadQueue.dequeue();
            if (!a.file.isEmpty()) {
                QFile f(a.file);
                UploaderSingleton::inst().upload(f, a.format);
            }
            if (a.postUploadTask) a.postUploadTask();
        }
    }
}

void RecordingController::startWithArea(QRect newArea) {
    area = newArea;
    preview = new RecordingPreview(newArea);
    timer.start(1000 / settings::settings().value("recording/framerate", 30).toInt());
}
