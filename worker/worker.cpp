#include "worker.hpp"
#include <chrono>
#include <thread>

Worker *Worker::inst = 0;
QMutex Worker::workerLock;
QMutex Worker::lock;

// QPixmaps don't like existing on non GUI threads.
// Because of this we have to:
// 1. Convert to image on the GUI thread
// 2. Queue onto the worker, where:
//  1. Convert the image to the right format
//  2. Consume the image.
void Worker::queue(WorkerContext *context) {
    init();
    QMutexLocker ml(&lock);
    _WorkerContext *c = new _WorkerContext;
    c->image = context->pixmap.toImage();
    c->consumer = context->consumer;
    c->targetFormat = context->targetFormat;
    c->underlyingThing = context;
    inst->qqueue.enqueue(std::move(c));
}

void Worker::init() {
    QMutexLocker ml(&workerLock);
    if (!inst) inst = new Worker;
}

Worker::Worker() : QObject() {
    thr = new QThread;
    moveToThread(thr);
    connect(thr, &QThread::started, this, &Worker::process);
    connect(thr, &QThread::finished, thr, &QThread::deleteLater);
    connect(this, &Worker::finished, this, &Worker::deleteLater);
    connect(thr, &QThread::finished, thr, &QThread::deleteLater);
    thr->start();
}

Worker::~Worker() {
    _end();
}

void Worker::end() {
    inst->_end();
}

void Worker::_end() {
    QMutexLocker ml(&endLock);
    _ended = true;
}

bool Worker::ended() {
    QMutexLocker ml(&endLock);
    return _ended;
}

void Worker::process() {
    while (!ended()) {
        lock.lock();
        if (!qqueue.isEmpty()) {
            _WorkerContext *c = qqueue.dequeue();
            lock.unlock();
            c->consumer(c->image.convertToFormat(c->targetFormat));
            delete c->underlyingThing;
            delete c;
        } else
            lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // STL likes it's scopes
    }
    emit finished();
}
