#include "recordingformats.hpp"

#include <QBuffer>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>
#include <gif-h/gif.h>
#include <platformbackend.hpp>
#include <settings.hpp>
#include <time.h>
#include <unistd.h>

RecordingFormats::RecordingFormats(RecordingFormats::Format f) {
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

    if (path.isEmpty()) {
        validator = [] { return false; };
        return;
    }
    tmpDir = QDir(path);
    QString name
    = QString("KShareTemp-") + QString::number(PlatformBackend::inst().pid()) + "-" + QTime::currentTime().toString();
    tmpDir.mkdir(name);
    tmpDir.cd(name);
    switch (f) {
    case GIF: {
        iFormat = QImage::Format_RGBA8888;
        validator = [] { return true; };
        consumer = [&](QImage img) { frames.push_back(img); };
        finalizer = [&] {
            if (frames.size() == 0) return QByteArray();
            uint32_t d = 1000 / settings::settings().value("recording/framerate", 30).toInt();
            QImage &startImg = frames[0];
            GifWriter writer;
            GifBegin(&writer, tmpDir.absoluteFilePath("resulting.gif").toLocal8Bit().constData(), startImg.width(),
                     startImg.height(), d);
            for (QImage &a : frames) {
                QByteArray alpha8((char *)a.bits(), a.byteCount());
                GifWriteFrame(&writer, (uint8_t *)alpha8.data(), a.width(), a.height(), d);
            }
            GifEnd(&writer);
            QFile res(tmpDir.absoluteFilePath("resulting.gif"));
            if (!res.open(QFile::ReadOnly)) {
                return QByteArray();
            }
            QByteArray data = res.readAll();
            return data;
        };
        break;
    }
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

std::function<bool()> RecordingFormats::getValidator() {
    return validator;
}

QImage::Format RecordingFormats::getFormat() {
    return iFormat;
}

QString RecordingFormats::getExt(RecordingFormats::Format f) {
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
