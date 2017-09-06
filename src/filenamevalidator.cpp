#include "filenamevalidator.hpp"

#include <formatter.hpp>
#include <platformbackend.hpp>

FilenameValidator::FilenameValidator(QObject *parent) : QValidator(parent) {
}

QValidator::State FilenameValidator::validate(QString &input, int &) const {
    QString name = formatter::format(input, "lol");
    return PlatformBackend::inst().filenameValid(name) ? State::Acceptable : State::Invalid;
}
