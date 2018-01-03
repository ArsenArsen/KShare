#include "recordingcontroller.hpp"
#include "../platformbackend.hpp"
#include "../uploaders/uploadersingleton.hpp"
#include <QImage>
#include <QRect>
#include <QTemporaryFile>
#include <QTimer>
#include <iostream>
#include <logger.hpp>
#include <mainwindow.hpp>
#include <screenareaselector/screenareaselector.hpp>
#include <settings.hpp>
#include <stdio.h>
#include <uploaders/uploadersingleton.hpp>
#include <utils.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
}

_Worker::_Worker(QRect area, QString output, RecordingController *controller)
: controller(controller), output(output), area(area) {
    exec();
}

void _Worker::run() {
    try {
        handler();
    } catch (std::exception &e) {
        QMetaObject::invokeMethod(controller, "error", Qt::QueuedConnection, Q_ARG(std::exception, e));
    }
}

void _Worker::handler() {
    std::runtime_error error("");
    bool errorPresent = false;
    AVOutputFormat *ofmt;
    AVFormatContext *iformat = nullptr, *oformat = nullptr;
    int *streamMapping = nullptr;
    try {
        int ret;
        // open device input
        PlatformBackend::inst().createFormatContext(&iformat, area);
        if ((ret = avformat_find_stream_info(iformat, NULL)) < 0)
            throw utils::av_error("Cannot find stream info! ", ret);

        // Format allocation
        ret = avformat_alloc_output_context2(&oformat, NULL, NULL, output.toLocal8Bit().constData());
        if (ret < 0) throw utils::av_error("output context allocation failed", ret);

        int mappingSize = (int)iformat->nb_streams;
        streamMapping = new int[mappingSize];
        ofmt = oformat->oformat;
        int streamIndex = 0;

        for (unsigned int i = 0; i < iformat->nb_streams; i++) {
            AVStream *ostream;
            AVStream *istream = iformat->streams[i];
            AVCodecParameters *icp = istream->codecpar;

            if (icp->codec_type != AVMEDIA_TYPE_VIDEO) {
                streamMapping[i] = -1;
                continue;
            }

            streamMapping[i] = streamIndex++;

            ostream = avformat_new_stream(oformat, NULL);
            if (!ostream) throw utils::av_error("Failed to allocate output stream");

            ret = avcodec_parameters_copy(ostream->codecpar, icp);
            if (ret < 0) throw utils::av_error("parameter copy failed", ret);
            ostream->codecpar->codec_tag = 0;
        }
        av_dump_format(oformat, 0, output.toLocal8Bit().constData(), 1);

        if (!(ofmt->flags & AVFMT_NOFILE)) {
            ret = avio_open(&oformat->pb, output.toLocal8Bit().constData(), AVIO_FLAG_WRITE);
            if (ret < 0) throw utils::av_error("failed to open output file", ret);
        }

        ret = avformat_write_header(oformat, NULL);
        if (ret < 0) throw utils::av_error("header failed to write", ret);
        AVPacket pkt;

        while (!isStopped()) {
            AVStream *in_stream, *out_stream;

            ret = av_read_frame(iformat, &pkt);
            if (ret < 0) break;

            in_stream = iformat->streams[pkt.stream_index];
            if (pkt.stream_index >= mappingSize || streamMapping[pkt.stream_index] < 0) {
                av_packet_unref(&pkt);
                continue;
            }

            pkt.stream_index = streamMapping[pkt.stream_index];
            out_stream = oformat->streams[pkt.stream_index];

            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                       static_cast<AVRounding>(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                       static_cast<AVRounding>(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.pos = -1;

            ret = av_interleaved_write_frame(oformat, &pkt);
            if (ret < 0) {
                av_packet_unref(&pkt);
                throw utils::av_error("failed to write frame: ", ret);
            }
            av_packet_unref(&pkt);
        }
        av_write_trailer(oformat);
    } catch (std::runtime_error e) {
        error = e;
        errorPresent = true;
    }
    avformat_close_input(&iformat);
    if (oformat && !(ofmt->flags & AVFMT_NOFILE)) avio_closep(&oformat->pb);
    avformat_free_context(oformat);
    delete[] streamMapping;
    if (errorPresent) throw error;
}

void _Worker::stop() {
    QMutexLocker locker(&interruptMutex);
    interrupt = true;
}

void _Worker::abort() {
    if (isStopped()) return;
    // yea im that lazy
    stop();
    QFile(output).remove();
}

bool _Worker::isStopped() {
    QMutexLocker locker(&interruptMutex);
    return interrupt;
}

// Controller

RecordingController::RecordingController() {
}

bool RecordingController::isRunning() {
    return worker && !worker->isStopped();
}

void RecordingController::start(formats::Recording format) {
    this->format = format;
    ScreenAreaSelector *sel = new ScreenAreaSelector;
    connect(sel, &ScreenAreaSelector::selectedArea, this, &RecordingController::startWithArea);
}

void RecordingController::startWithArea(QRect area) {
    QTemporaryFile file("KShare-XXXXXX." + formats::recordingFormatName(format).toLower());
    file.setAutoRemove(false);
    output = QFileInfo(file).absoluteFilePath();
    worker = new _Worker(area, output, this);
    worker->start();
}

void RecordingController::error(std::exception e) {
    logger::fatal(QString("Recording error: ") + e.what());
    worker->wait();
    delete worker;
    worker = nullptr;
}

void RecordingController::stop() {
    worker->stop();
    worker->wait();
    delete worker;
    worker = nullptr;
    QFile o(output);
    UploaderSingleton::inst().upload(o, QFileInfo(o).completeSuffix());
}

void RecordingController::abort() {
    worker->abort();
    worker->wait();
    delete worker;
    worker = nullptr;
}
