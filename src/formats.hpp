#ifndef FORMATS_HPP
#define FORMATS_HPP

#include <QString>

namespace formats {
    enum class Normal { PNG, JPG, None };
    QString normalFormatName(Normal format);
    Normal normalFormatFromName(QString format);
    QString normalFormatMIME(Normal format);

    enum class Recording { GIF, WebM, MP4, None };
    QString recordingFormatName(Recording format);
    Recording recordingFormatFromName(QString format);
    QString recordingFormatMIME(Recording format);
} // namespace formats
#endif // FORMATS_HPP
