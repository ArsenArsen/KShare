#ifndef RECORDINGFORMATS_HPP
#define RECORDINGFORMATS_HPP

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>
#include <formats.hpp>
#include <functional>

class RecordingFormats {
public:
    RecordingFormats(formats::Recording f);
    std::function<void(QImage)> getConsumer();
    std::function<QByteArray()> getFinalizer();
    std::function<bool()> getValidator();
    QImage::Format getFormat();
    QString getAnotherFormat();

private:
    std::function<void(QImage)> consumer;
    std::function<bool()> validator;
    std::function<QByteArray()> finalizer;
    std::vector<QImage> frames;
    QImage::Format iFormat;
    QDir tmpDir;
    QString anotherFormat;
};

#endif // RECORDINGFORMATS_HPP
