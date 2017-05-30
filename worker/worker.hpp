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
};

class Worker : public QObject {
    Q_OBJECT
public:
    void queue(WorkerContext *context);
    static void init();

private:
    Worker();
    ~Worker();
    QMutex lock;
    QMutex endLock;
    QThread *thr;
    QQueue<_WorkerContext *> qqueue; // Say that ten times as fast
    bool _ended;

    void _queue(WorkerContext *context);
    void end();
    bool ended();

    static Worker *inst;
    static QMutex workerLock;
signals:
    void error(QString err);
    void finished();

public slots:
    void process();
};

#endif // WORKER_HPP
