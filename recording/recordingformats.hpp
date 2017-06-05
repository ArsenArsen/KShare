#ifndef RECORDINGFORMATS_HPP
#define RECORDINGFORMATS_HPP

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>
#include <functional>

class RecordingFormats {
public:
    enum Format { GIF, None };
    RecordingFormats(Format f);
    std::function<void(QImage)> getConsumer();
    std::function<QByteArray()> getFinalizer();
    std::function<bool()> getValidator();
    QImage::Format getFormat();

    static QString getExt(Format f);

private:
    std::function<void(QImage)> consumer;
    std::function<bool()> validator;
    std::function<QByteArray()> finalizer;
    std::vector<QImage> frames;
    QImage::Format iFormat;
    QDir tmpDir;
    int frame = 0;
};

#endif // RECORDINGFORMATS_HPP
