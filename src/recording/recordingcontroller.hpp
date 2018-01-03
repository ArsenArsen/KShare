#ifndef RECORDINGCONTROLLER_HPP
#define RECORDINGCONTROLLER_HPP


#include <QMutex>
#include <QMutexLocker>
#include <QRect>
#include <QThread>
#include <formats.hpp>

class RecordingController;

class _Worker : public QThread {
    Q_OBJECT
public:
    // Sets variables, executes, starts immediately
    _Worker(QRect recordingArea, QString output, RecordingController *c);
    // Thread-safe controls

    // Stops recording, writes header, closes devices.
    void stop();
    bool isStopped();
    // Abruptly aborts recording, cleans up, deletes output
    void abort();

protected:
    void run() override;

private:
    void handler();
    RecordingController *controller;
    QString output;
    QRect area;
    bool interrupt;
    QMutex interruptMutex;
};

class RecordingController : public QObject {
    Q_OBJECT
public:
    RecordingController();
    bool isRunning();
public slots:
    void start(formats::Recording format);
    void startWithArea(QRect newArea);
    void stop();
    void abort();
    void error(std::exception e);

private:
    _Worker *worker = nullptr;
    QString output;
    formats::Recording format;
};

#endif // RECORDINGCONTROLLER_HPP
