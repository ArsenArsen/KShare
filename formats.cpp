#include <formats.hpp>

QString formats::normalFormatName(formats::Normal format) {
    switch (format) {
    case Normal::JPG:
        return "JPG";
        break;
    case Normal::PNG:
        return "PNG";
        break;
    default:
        return QString();
        break;
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
        break;
    case Normal::PNG:
        return "image/png";
        break;
    default:
        return QString();
        break;
    }
}

QString formats::recordingFormatName(formats::Recording format) {
    switch (format) {
    case Recording::GIF:
        return "GIF";
        break;
    case Recording::WebM:
        return "WEBM";
        break;
    default:
        return QString();
        break;
    }
}

formats::Recording formats::recordingFormatFromName(QString format) {
    if (format.toLower() == "gif") return Recording::GIF;
    if (format.toLower() == "webm") return Recording::WebM;
    return Recording::None;
}

QString formats::recordingFormatMIME(formats::Recording format) {
    switch (format) {
    case Recording::GIF:
        return "image/gif";
        break;
    case Recording::WebM:
        return "video/webm";
        break;
    default:
        return QString();
        break;
    }
}
