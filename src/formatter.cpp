#include "formatter.hpp"

#include <QDateTime>
#include <QStringList>

QString formatter::format(QString toFormat, QString ext, QMap<QString, QString> variables) {
    QString formatted(toFormat);

    QRegExp dateRegex("%(?!%)\\((.+)\\)date");
    dateRegex.indexIn(toFormat);
    QStringList capturedTexts(dateRegex.capturedTexts());
    QDateTime date = QDateTime::currentDateTime();
    for (int i = 0; i < capturedTexts.length(); i += 2) {
        formatted = formatted.replace(capturedTexts.at(i), date.toString(capturedTexts.at(i + 1)));
    }

    QRegExp randomRegex("%(?!%)\\((.+)\\)random");
    randomRegex.indexIn(toFormat);
    QStringList randomTexts(randomRegex.capturedTexts());
    for (int i = 0; i < randomTexts.length(); i += 2) {
        QStringList list = randomTexts.at(i + 1).split('|');
        formatted = formatted.replace(randomTexts.at(i), list.at(rand() % (list.length())));
    }

    for (QString var : variables.keys()) formatted.replace("%" + var, variables[var]);

    formatted = formatted.replace(QRegExp("%(?!%)ext"), ext);
    return formatted;
}
