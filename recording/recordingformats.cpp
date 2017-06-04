#include "recordingformats.hpp"

#include <QFile>

RecordingFormats::RecordingFormats(RecordingFormats::Format f) {
    switch (f) {
    case GIF:
        // TODO
        break;
    default:
        break;
    }
}

std::function<void(QImage)> RecordingFormats::getConsumer() {
    return consumer;
}

std::function<QByteArray()> RecordingFormats::getFinalizer() {
    return finalizer;
}

QString RecordingFormats::getPrettyName(RecordingFormats::Format f) {
    switch (f) {
    case None:
        return "None";
        break;
    case GIF:
        return "gif";
        break;
    default:
        return QString();
        break;
    }
}
