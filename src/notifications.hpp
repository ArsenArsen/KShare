#ifndef NOTIFICATIONS_HPP
#define NOTIFICATIONS_HPP

#include <QString>
#include <QSystemTrayIcon>

namespace notifications {
    void notify(QString title, QString body, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);
    void notifyNolog(QString title, QString body, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);
} // namespace notifications

#endif // NOTIFICATIONS_HPP
