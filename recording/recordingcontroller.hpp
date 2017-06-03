#ifndef RECORDINGCONTROLLER_HPP
#define RECORDINGCONTROLLER_HPP

#include "recordingpreview.hpp"

#include <QImage>
#include <QRect>
#include <QTimer>
#include <functional>
#include <memory>

class RecordingContext {
public:
    QImage::Format format;
    std::function<void(QImage)> consumer;
    std::function<void()> finalizer;
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
private slots:
    void timeout();
    void startWithArea(QRect newArea);

private:
    QRect area;
    RecordingContext *_context = 0;
    QTimer timer;
    RecordingPreview *preview = nullptr;
    unsigned int frame = 0;
    unsigned int time = 0;
};

#endif // RECORDINGCONTROLLER_HPP
