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

class RecordingContext {
public:
    QImage::Format format;
    std::function<void(QImage)> consumer;
    std::function<bool()> validator;
    std::function<QByteArray()> finalizer;
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
    void queue(QByteArray arr);
private slots:
    void timeout();
    void startWithArea(QRect newArea);

private:
    QMutex lock;
    QQueue<QByteArray> uploadQueue;
    QRect area;
    RecordingContext *_context = 0;
    QTimer timer;
    RecordingPreview *preview = nullptr;
    unsigned int frame = 0;
    unsigned int time = 0;
};

#endif // RECORDINGCONTROLLER_HPP
