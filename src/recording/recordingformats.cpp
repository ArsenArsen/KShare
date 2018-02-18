#include "recordingformats.hpp"

#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>
#include <formats.hpp>
#include <logger.hpp>
#include <notifications.hpp>
#include <platformbackend.hpp>
#include <settings.hpp>
#include <time.h>
#include <unistd.h>

#include <recording/encoders/encodersettings.hpp>

RecordingFormats::RecordingFormats(formats::Recording f) {
    if (!tmpDir.isValid()) {
        validator = [](QSize) { return false; };
        logger::fatal(tr("Could not create temporary directory. Error: ") + tmpDir.errorString());
        return;
    }
    iFormat = QImage::Format_RGB888;
    path = tmpDir.path() + "/res." + formats::recordingFormatName(f).toLower();
    finalizer = [&] {
        delete enc;
        return QFile(path).size() > 0 ? path : QString();
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
            } catch (std::runtime_error &e) {
                logger::fatal(tr("Encoder error: ") + e.what());
                interrupt = true;
                delete enc;
                return false;
            }
        }
        return !interrupt;
    };
    consumer = [&](QImage img) {
        if (!interrupt) try {
                frameAdded = true;
                enc->addFrame(img);
            } catch (std::runtime_error &e) {
                logger::fatal(tr("Encoder error: ") + e.what());
                interrupt = true;
            }
    };
    postUploadTask = [&] { QScopedPointer<RecordingFormats> th(this); };
    anotherFormat = formats::recordingFormatName(f);
}

std::function<void(QImage)> RecordingFormats::getConsumer() {
    return consumer;
}

std::function<QString()> RecordingFormats::getFinalizer() {
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
