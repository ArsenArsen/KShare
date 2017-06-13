#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <QImage>
#include <QSize>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

struct OutputStream {
    AVStream *st = NULL;
    AVCodecContext *enc = NULL;

    int64_t nextPts = 0;

    AVFrame *frame = NULL;

    SwsContext *sws = NULL;
};

class Encoder {
public:
    Encoder(QString &targetFile, QSize res);
    ~Encoder();
    bool addFrame(QImage frm);
    bool isRunning();
    bool end();

private:
    AVCodec *codec = NULL;

    OutputStream *out = new OutputStream;
    AVFormatContext *fc = NULL;

    bool success = false;

    QSize size;

    void setFrameRGB(uint8_t *rgb);
};

#endif // ENCODER_HPP
