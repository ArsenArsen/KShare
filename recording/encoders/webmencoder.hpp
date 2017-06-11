#ifndef WEBMENCODER_HPP
#define WEBMENCODER_HPP

#include <QImage>
#include <QSize>
#include <formats.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class WebMEncoder {
public:
    static constexpr AVCodecID CODEC = AV_CODEC_ID_VP8;
    static constexpr formats::Recording FORMAT = formats::Recording::WebM;

    WebMEncoder(QSize res);
    ~WebMEncoder();
    bool addFrame(QImage frm);
    bool isRunning();
    QByteArray end();

private:
    AVCodec *codec = NULL;
    AVCodecContext *c = NULL;
    AVFrame *frame = NULL;
    AVPacket pkt;

    bool success = false;

    QByteArray video;
    QSize size;
    struct SwsContext *sws_context = NULL;

    void setFrameRGB(uint8_t *rgb);
};

#endif // WEBMENCODER_HPP
