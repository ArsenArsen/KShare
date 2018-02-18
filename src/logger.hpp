#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QString>

namespace logger {
    void info(QString info);  // Something went okay, notify user about it
    void warn(QString info);  // Something that does not interrupt execution, but could be improved
    void error(QString info); // Oh no - it errored, but is recoverable, and still important enough to notify
    void fatal(QString info); // Unrecoverable error, procedure aborts and notifies the user
    void abort(QString info); // tell the user that it gonna ded
} // namespace logger

#endif /* LOGGER_HPP */
