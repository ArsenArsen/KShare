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

struct CodecSettings {
    int bitrate;
    int gopSize;
    int bFrames;
    int mbDecisions;
    QString h264Profile;
    int h264Crf;
    bool vp9Lossless;
};

class Encoder {
public:
    Encoder(QString &targetFile, QSize res, CodecSettings *settings = NULL);
    ~Encoder();
    bool addFrame(QImage frm);
    bool isRunning();
    bool end();

private:
    AVCodec *codec = NULL;

    OutputStream *out = new OutputStream;
    AVFormatContext *fc = NULL;

    bool success = false;
    bool ended = false;

    QSize size;

    void setFrameRGB(QImage img);
};

#endif // ENCODER_HPP
