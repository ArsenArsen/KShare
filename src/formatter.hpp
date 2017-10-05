#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <QMap>
#include <QRegExp>
#include <QString>

namespace formatter {
    QString format(QString toFormat, QString ext, QMap<QString, QString> variables = QMap<QString, QString>());
}

#endif // FORMATTER_HPP
