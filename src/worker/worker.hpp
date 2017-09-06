#ifndef WORKER_HPP
#define WORKER_HPP

#include <QImage>
#include <QMutex>
#include <QObject>
#include <QPixmap>
#include <QQueue>
#include <QThread>
#include <functional>

struct WorkerContext {
    QPixmap pixmap;
    QImage::Format targetFormat;
    std::function<void(QImage)> consumer;
};

struct _WorkerContext {
    QImage image;
    QImage::Format targetFormat;
    std::function<void(QImage)> consumer;
    WorkerContext *underlyingThing;
};

class Worker : public QObject {
    Q_OBJECT
public:
    static void queue(WorkerContext *context);
    static void init();

private:
    Worker();
    ~Worker();
    static QMutex lock;
    QMutex endLock;
    QThread *thr;
    QQueue<_WorkerContext *> qqueue; // Say that ten times as fast
    bool _ended;
    void _end();

    void _queue(WorkerContext *context);
    bool ended();

    static Worker *inst;
    static QMutex workerLock;
signals:
    void error(QString err);
    void finished();

public slots:
    void process();
    static void end();
};

#endif // WORKER_HPP
