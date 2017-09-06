#ifndef FILENAMEVALIDATOR_HPP
#define FILENAMEVALIDATOR_HPP

#include <QValidator>

class FilenameValidator : public QValidator {
public:
    FilenameValidator(QObject *parent = nullptr);
    QValidator::State validate(QString &input, int &) const override;
};

#endif // FILENAMEVALIDATOR_HPP
