#include "formatter.hpp"

#include <QDateTime>
#include <QStringList>

QString formatter::format(QString toFormat)
{
    QRegExp dateRegex("%\\((.+)\\)date");
    dateRegex.indexIn(toFormat);
    QStringList capturedTexts(dateRegex.capturedTexts());
    QString formatted(toFormat);
    QDateTime date = QDateTime::currentDateTime();
    for (int i = 0; i < capturedTexts.length(); i += 2)
    {
        formatted = formatted.replace(capturedTexts.at(i), date.toString(capturedTexts.at(i + 1)));
    }
    return formatted;
}
