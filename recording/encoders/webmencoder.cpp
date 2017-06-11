#include "webmencoder.hpp"

#include <settings.hpp>
extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

inline void throwAVErr(int ret) {
    char err[AV_ERROR_MAX_STRING_SIZE];
    av_make_error_string(err, AV_ERROR_MAX_STRING_SIZE, ret);
    std::string newString(err);
    throw std::runtime_error(newString);
}

WebMEncoder::WebMEncoder(QSize res) {
    codec = avcodec_find_encoder(CODEC);
    if (!codec) throw std::runtime_error("Codec not found");
    c = avcodec_alloc_context3(codec);
    if (!c) throw std::runtime_error("Unable to allocate video context");
    c->bit_rate = 400000;

    c->width = res.width() % 2 ? res.width() - 1 : res.width();
    c->height = res.height() % 2 ? res.height() - 1 : res.height();
    size = QSize(c->width, c->height);
    int fps = settings::settings().value("recording/framerate", 30).toInt();
    c->time_base = { 1, fps };
    c->framerate = { fps, 1 };
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    int ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) throwAVErr(ret);

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width = c->width;
    frame->height = c->height;
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height, c->pix_fmt, 32);
    if (!ret) throwAVErr(ret);

    success = true;
}

void WebMEncoder::setFrameRGB(uint8_t *rgb) {
    int lineSize[1] = { 3 * c->width };
    sws_context = sws_getCachedContext(sws_context, c->width, c->height, AV_PIX_FMT_RGB24, c->width, c->height,
                                       AV_PIX_FMT_YUV420P, 0, 0, 0, 0);
    sws_scale(sws_context, (const uint8_t *const *)&rgb, lineSize, 0, c->height, frame->data, frame->linesize);
}

WebMEncoder::~WebMEncoder() {
    end();
}

bool WebMEncoder::addFrame(QImage frm) {
    if (!success) return false;
    if (frm.size() != size) frm = frm.copy(QRect(QPoint(0, 0), size));
    if (frm.format() != QImage::Format_RGB888) frm = frm.convertToFormat(QImage::Format_RGB888);
    uint8_t *frameData = (uint8_t *)frm.bits();
    setFrameRGB(frameData);
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;
    int ret = avcodec_send_frame(c, frame);
    if (ret < 0) return false;
    while (ret >= 0) {
        ret = avcodec_receive_packet(c, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return true;
        else if (ret < 0) {
            return false;
        }
        video.append((const char *)pkt.data, pkt.size);
        av_packet_unref(&pkt);
    }
    return true;
}

bool WebMEncoder::isRunning() {
    return success;
}

QByteArray WebMEncoder::end() {
    int ret;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    if (!success) {
        goto cleanup;
    }
    ret = avcodec_send_frame(c, frame);
    while (ret >= 0) {
        ret = avcodec_receive_packet(c, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0)
            break;
        video.append((const char *)pkt.data, pkt.size);
        av_packet_unref(&pkt);
    }
    video.append((const char *)endcode, sizeof(endcode));
cleanup:
    if (c) {
        avcodec_close(c);
        avcodec_free_context(&c);
    }
    if (frame) av_frame_free(&frame);
    av_packet_unref(&pkt);
    return video;
}
