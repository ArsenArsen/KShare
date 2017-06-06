#ifndef FORMATS_HPP
#define FORMATS_HPP

#include <QString>

namespace formats {
enum class Normal { PNG, JPG, None };
QString normalFormatName(Normal format);
Normal normalFormatFromName(QString format);
QString normalFormatMIME(Normal format);

enum class Recording { GIF, None };
QString recordingFormatName(Recording format);
Recording recordingFormatFromName(QString format);
QString recordingFormatMIME(Recording format);
}
#endif // FORMATS_HPP
