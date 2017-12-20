#include "recordingcontroller.hpp"
#include "../platformbackend.hpp"
#include <QImage>
#include <QRect>
#include <QTimer>
#include <iostream>
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
}

_Worker::_Worker(QRect area, QString output, RecordingController *controller)
: controller(controller), area(area), output(output) {
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
    AVFormatContext *iformat, *oformat;
    int ret;
    // open device input
    PlatformBackend::inst().createFormatContext(&iformat, area);
    if ((ret = avformat_find_stream_info(iformat, NULL)) < 0) throw utils::av_error("Cannot find stream info! ", ret);
    for (int i = 0; i < iformat->nb_streams; i++) {
        if (iformat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            
    }
    if (!iformat->video_codec) throw std::runtime_error("No video stream found");
}

RecordingController::RecordingController() {
}
