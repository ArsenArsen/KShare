#include "encoder.hpp"

#include <settings.hpp>
extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

inline void throwAVErr(int ret, std::string section) {
    char err[AV_ERROR_MAX_STRING_SIZE];
    av_make_error_string(err, AV_ERROR_MAX_STRING_SIZE, ret);
    std::string newString(err);
    throw std::runtime_error("Error during: " + section + ": " + newString);
}

Encoder::Encoder(QString &targetFile, QSize res) {
    int ret;
    // Format
    ret = avformat_alloc_output_context2(&fc, NULL, NULL, targetFile.toLocal8Bit().constData());
    if (ret < 0) throwAVErr(ret, "format alloc");

    // Stream
    out->st = avformat_new_stream(fc, NULL);
    if (!out->st) throw std::runtime_error("Unable to allocate video context");
    out->st->id = fc->nb_streams - 1;
    if (fc->oformat->flags & AVFMT_GLOBALHEADER) fc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    av_dump_format(fc, 0, targetFile.toLocal8Bit().constData(), 1);

    // Codec
    if (!fc->oformat->video_codec) throw std::runtime_error("Video codec not found");
    codec = avcodec_find_encoder(fc->oformat->video_codec);
    if (!codec) throw std::runtime_error("Video codec not found");
    out->enc = avcodec_alloc_context3(codec);
    if (!out->enc) throw std::runtime_error("Unable to allocate video context");

    int fps = settings::settings().value("recording/framerate", 30).toInt();

    out->enc->codec_id = codec->id;

    out->enc->bit_rate = 400000;
    out->enc->width = res.width() % 2 ? res.width() - 1 : res.width();
    out->enc->height = res.height() % 2 ? res.height() - 1 : res.height();
    size = QSize(out->enc->width, out->enc->height);
    out->st->time_base = { 1, fps };
    out->enc->time_base = out->st->time_base;

    out->enc->gop_size = 12;
    out->enc->pix_fmt = AV_PIX_FMT_YUV420P; // blaze it
    if (out->enc->codec_id == AV_CODEC_ID_MPEG2VIDEO)
        out->enc->max_b_frames = 2;
    else if (out->enc->codec_id == AV_CODEC_ID_MPEG1VIDEO)
        out->enc->mb_decision = 2;


    ret = avcodec_open2(out->enc, codec, NULL);
    if (ret < 0) throwAVErr(ret, "codec open");

    ret = avcodec_parameters_from_context(out->st->codecpar, out->enc);
    if (ret < 0) throwAVErr(ret, "stream opt copy");

    // Frames
    out->frame = av_frame_alloc();
    if (!out->frame) {
        throw std::runtime_error("frame alloc");
    }
    out->frame->format = out->enc->pix_fmt;
    out->frame->width = out->enc->width;
    out->frame->height = out->enc->height;
    ret = av_frame_get_buffer(out->frame, 32);
    if (ret < 0) throwAVErr(ret, "frame buffer alloc");

    // Writer
    ret = avio_open(&fc->pb, targetFile.toLocal8Bit().constData(), AVIO_FLAG_WRITE);
    if (ret < 0) throwAVErr(ret, "writer open");
    ret = avformat_write_header(fc, NULL);
    if (ret < 0) throwAVErr(ret, "write header");

    success = true;
}

void Encoder::setFrameRGB(uint8_t *rgb) {
    int ret = av_frame_make_writable(out->frame);
    if (ret < 0) throwAVErr(ret, "setFrameRGB");
    int lineSize[1] = { 3 * out->enc->width };
    out->sws = sws_getCachedContext(out->sws, out->enc->width, out->enc->height, AV_PIX_FMT_RGB24, out->enc->width,
                                    out->enc->height, (AVPixelFormat)out->frame->format, 0, 0, 0, 0);
    sws_scale(out->sws, (const uint8_t *const *)&rgb, lineSize, 0, out->enc->height, out->frame->data, out->frame->linesize);
    out->frame->pts = out->nextPts++;
}

Encoder::~Encoder() {
    end();
}

bool Encoder::addFrame(QImage frm) {
    if (!success) return false;
    if (frm.size() != size) frm = frm.copy(QRect(QPoint(0, 0), size));
    if (frm.format() != QImage::Format_RGB888) frm = frm.convertToFormat(QImage::Format_RGB888);
    uint8_t *frameData = (uint8_t *)frm.bits();
    setFrameRGB(frameData);
    av_init_packet(&pkt);
    int gotPack = 0;
    int ret = avcodec_encode_video2(out->enc, &pkt, out->frame, &gotPack);
    if (ret < 0) {
        av_packet_unref(&pkt);
        throwAVErr(ret, "encode video");
    }

    if (gotPack) {
        av_packet_rescale_ts(&pkt, out->enc->time_base, out->st->time_base);
        pkt.stream_index = out->st->index;
        ret = av_interleaved_write_frame(fc, &pkt);
    } else
        ret = 0;
    av_packet_unref(&pkt);
    if (ret < 0) throwAVErr(ret, "write frame");
    return true;
}

bool Encoder::isRunning() {
    return success;
}

bool Encoder::end() {
    if (!success) {
        goto cleanup;
    }

    av_write_trailer(fc);
cleanup:
    avcodec_free_context(&out->enc);
    av_frame_free(&out->frame);
    sws_freeContext(out->sws);
    delete out;
    if (!(fc->oformat->flags & AVFMT_NOFILE)) avio_closep(&fc->pb);
    avformat_free_context(fc);
    return success;
}