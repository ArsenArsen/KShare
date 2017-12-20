#ifndef RECORDINGCONTROLLER_HPP
#define RECORDINGCONTROLLER_HPP

#include "recordingpreview.hpp"

#include <QRect>
#include <QThread>

class RecordingController;

class _Worker : public QThread {
    Q_OBJECT
public:
    // Sets variables, executes, starts immediately
    _Worker(QRect recordingArea, QString output, RecordingController *c);
    // Thread-safe controls

    // Stops recording, writes header, closes devices.
    void stop();
    // Abruptly aborts recording, cleans up, deletes output
    void abort();
protected:
    void run() override;
private:
    void handler();
    RecordingController *controller;
    QString output;
    QRect area;
};

class RecordingController : public QThread {
public:
    RecordingController();
    bool isRunning();
public slots:
    void startWithArea(QRect newArea);
    void start();
    void stop();
    void abort();
    void error(std::exception e);

private:
    _Worker *worker;
    quint64 frame = 0;
    quint64 time = 0;
signals:
    void ended();
};

#endif // RECORDINGCONTROLLER_HPP
