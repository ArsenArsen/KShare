#ifndef RECORDINGFORMATS_HPP
#define RECORDINGFORMATS_HPP

#include <QFile>
#include <QImage>
#include <QString>
#include <functional>

class RecordingFormats {
public:
    enum Format { None, GIF };
    RecordingFormats(Format f);
    std::function<void(QImage)> getConsumer();
    std::function<QByteArray()> getFinalizer();

    static QString getPrettyName(Format f);

private:
    std::function<void(QImage)> consumer;
    std::function<QByteArray()> finalizer;
};

#endif // RECORDINGFORMATS_HPP
