#include "encodersettings.hpp"

#undef SETTINGS_INTERFACE

#define SETTINGS_FIELD(f, n, d, t) f = settings::settings().value(n, d).value<t>();
#define SETTINGS_INTERFACE(F, t)                                                                                       \
    t EncoderSettings::get##F() {                                                                                      \
        return F;                                                                                                      \
    }                                                                                                                  \
    void EncoderSettings::set##F(t newf) {                                                                             \
        F = newf;                                                                                                      \
    }

EncoderSettings::EncoderSettings() {
    SETTINGS_FIELD(bitrate, "codec/bitrate", 400000, int);
    SETTINGS_FIELD(gopSize, "codec/gopsize", 12, int);
    SETTINGS_FIELD(h264Profile, "codec/h264Profile", "medium", QString);
    SETTINGS_FIELD(h264Crf, "codec/h264Crf", 23, int);
    SETTINGS_FIELD(vp9Lossless, "codec/vp9Lossless", false, bool);
    SETTINGS_FIELD(imageQuality, "imageQuality", -1, int);
}

SETTINGS_INTERFACE(bitrate, int)
SETTINGS_INTERFACE(gopSize, int)
SETTINGS_INTERFACE(h264Profile, QString)
SETTINGS_INTERFACE(h264Crf, int)
SETTINGS_INTERFACE(vp9Lossless, int)
SETTINGS_INTERFACE(imageQuality, int)

EncoderSettings EncoderSettings::inst() {
    static EncoderSettings e;
    return e;
}

#define SETTINGS_S_S(se) s->se = se;

CodecSettings *EncoderSettings::getSettings() {
    auto s = new CodecSettings;
    SETTINGS_S_S(bitrate)
    SETTINGS_S_S(gopSize)
    SETTINGS_S_S(h264Profile)
    SETTINGS_S_S(h264Crf)
    SETTINGS_S_S(vp9Lossless)
    return s;
}
