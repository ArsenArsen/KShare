#ifndef RECORDINGCONTROLLER_HPP
#define RECORDINGCONTROLLER_HPP

#include "recordingpreview.hpp"

#include <QFile>
#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QRect>
#include <QTimer>
#include <functional>
#include <memory>

struct RecordingContext {
    QImage::Format format;
    std::function<void(QImage)> consumer;
    std::function<bool(QSize)> validator;
    std::function<QString()> finalizer;
    std::function<void()> postUploadTask;
    QString anotherFormat;
};

struct _QueueContext {
    QString file;
    QString format;
    std::function<void()> postUploadTask;
};

class RecordingController : public QObject {
    Q_OBJECT
public:
    RecordingController();
    bool isRunning();
public slots:
    // Returns false if isRunning
    bool start(RecordingContext *context);
    // Returns false if not running
    bool end();
    void queue(_QueueContext arr);
    bool abort();
private slots:
    void timeout();
    void startWithArea(QRect newArea);

private:
    QMutex lock;
    QMutex timerl;
    QQueue<_QueueContext> uploadQueue;
    QRect area;
    RecordingContext *_context = 0;
    QTimer timer;
    RecordingPreview *preview = nullptr;
    unsigned int frame = 0;
    unsigned int time = 0;
signals:
    void ended();
};

#endif // RECORDINGCONTROLLER_HPP
