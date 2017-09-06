#ifndef RECORDINGFORMATS_HPP
#define RECORDINGFORMATS_HPP

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>
#include <QTemporaryDir>
#include <formats.hpp>
#include <functional>

#include <recording/encoders/encoder.hpp>

class RecordingFormats {
    Q_DECLARE_TR_FUNCTIONS(RecordingFormats)
public:
    RecordingFormats(formats::Recording f);
    std::function<void(QImage)> getConsumer();
    std::function<bool(QSize)> getValidator();
    std::function<QString()> getFinalizer();
    std::function<void()> getPostUploadTask();
    QImage::Format getFormat();
    QString getAnotherFormat();

private:
    std::function<void(QImage)> consumer;
    std::function<bool(QSize)> validator;
    std::function<QString()> finalizer;
    std::function<void()> postUploadTask;
    std::vector<QImage> frames;
    QImage::Format iFormat;
    QTemporaryDir tmpDir;
    QString path;
    Encoder *enc = NULL;
    bool interrupt = false;
    bool frameAdded = false;
    QString anotherFormat;
};

#endif // RECORDINGFORMATS_HPP
