#include <formats.hpp>

QString formats::normalFormatName(formats::Normal format) {
    switch (format) {
    case Normal::JPG:
        return "JPG";
    case Normal::PNG:
        return "PNG";
    default:
        return QString();
    }
}

formats::Normal formats::normalFormatFromName(QString format) {
    if (format.toLower() == "jpg") return Normal::JPG;
    if (format.toLower() == "jpeg") return Normal::JPG;
    if (format.toLower() == "png") return Normal::PNG;
    return Normal::None;
}

QString formats::normalFormatMIME(formats::Normal format) {
    switch (format) {
    case Normal::JPG:
        return "image/jpeg";
    case Normal::PNG:
        return "image/png";
    default:
        return QString();
    }
}

QString formats::recordingFormatName(formats::Recording format) {
    switch (format) {
    case Recording::GIF:
        return "GIF";
    case Recording::WebM:
        return "WEBM";
    case Recording::MP4:
        return "MP4";
    default:
        return QString();
    }
}

formats::Recording formats::recordingFormatFromName(QString format) {
    if (format.toLower() == "gif") return Recording::GIF;
    if (format.toLower() == "webm") return Recording::WebM;
    if (format.toLower() == "mp4") return Recording::MP4;
    return Recording::None;
}

QString formats::recordingFormatMIME(formats::Recording format) {
    switch (format) {
    case Recording::GIF:
        return "image/gif";
    case Recording::WebM:
        return "video/webm";
    case Recording::MP4:
        return "video/mp4";
    default:
        return QString();
    }
}
