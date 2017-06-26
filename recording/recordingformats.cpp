#include "recordingformats.hpp"

#include <QBuffer>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>
#include <formats.hpp>
#include <notifications.hpp>
#include <platformbackend.hpp>
#include <settings.hpp>
#include <time.h>
#include <unistd.h>

#include <recording/encoders/encodersettings.hpp>

RecordingFormats::RecordingFormats(formats::Recording f) {
    QString tmp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (tmp.isEmpty()) {
        validator = [](QSize) { return false; };
        return;
    }
    tmpDir = QDir(tmp);
    QString name
    = QString("KShareTemp-") + QString::number(PlatformBackend::inst().pid()) + "-" + QTime::currentTime().toString();
    tmpDir.mkdir(name);
    tmpDir.cd(name);
    iFormat = QImage::Format_RGB888;
    path = tmpDir.absoluteFilePath("res." + formats::recordingFormatName(f).toLower());
    finalizer = [&] {
        delete enc;
        if (interrupt) {
            tmpDir.removeRecursively();
            return QByteArray();
        }
        QFile res(path);
        if (!res.open(QFile::ReadOnly)) {
            qCritical().noquote() << "Could not open resulting file: " << res.errorString();
            return QByteArray();
        }
        QByteArray data = res.readAll();
        return data;
    };
    validator = [&](QSize s) {
        if (!enc) {
            try {
                auto es = EncoderSettings::inst().getSettings();
                enc = new Encoder(path, s, es);
                delete es;
                if (!enc->isRunning()) {
                    delete enc;
                    return false;
                }
            } catch (std::runtime_error e) {
                //                notifications::notify("KShare Video Encoder Error", e.what(),
                //                QSystemTrayIcon::Critical);
                qCritical() << "Encoder error: " << e.what();
                interrupt = true;
                delete enc;
                return false;
            }
        }
        return !interrupt;
    };
    consumer = [&](QImage img) {
        if (!interrupt) try {
                enc->addFrame(img);
            } catch (std::runtime_error e) {
                //                notifications::notify("KShare Video Encoder Error", e.what(),
                //                QSystemTrayIcon::Critical);
                qCritical() << "Encoder error: " << e.what();
                interrupt = true;
            }
    };
    postUploadTask = [&] {
        tmpDir.removeRecursively();
        QScopedPointer<RecordingFormats> th(this);
    };
    anotherFormat = formats::recordingFormatName(f);
}

std::function<void(QImage)> RecordingFormats::getConsumer() {
    return consumer;
}

std::function<QByteArray()> RecordingFormats::getFinalizer() {
    return finalizer;
}

std::function<void()> RecordingFormats::getPostUploadTask() {
    return postUploadTask;
}

std::function<bool(QSize)> RecordingFormats::getValidator() {
    return validator;
}

QImage::Format RecordingFormats::getFormat() {
    return iFormat;
}

QString RecordingFormats::getAnotherFormat() {
    return anotherFormat;
}
