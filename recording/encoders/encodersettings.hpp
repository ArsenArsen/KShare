#ifndef ENCODERSETTINGS_HPP
#define ENCODERSETTINGS_HPP

#include "encoder.hpp"
#include <settings.hpp>
#define SETTINGS_INTERFACE(F, t)                                                                                       \
    t get##F();                                                                                                        \
    void set##F(t newf);

class EncoderSettings {
public:
    EncoderSettings();

    static EncoderSettings inst();
    CodecSettings *getSettings();
    SETTINGS_INTERFACE(bitrate, int)
    SETTINGS_INTERFACE(gopSize, int)
    SETTINGS_INTERFACE(h264Profile, QString)
    SETTINGS_INTERFACE(h264Crf, int)
    SETTINGS_INTERFACE(vp9Lossless, int)
    SETTINGS_INTERFACE(imageQuality, int)

private:
    int bitrate;
    int gopSize;
    QString h264Profile;
    int h264Crf;
    int imageQuality;
    bool vp9Lossless;
};

#endif // ENCODERSETTINGS_HPP
