#include "notifications.hpp"

#include "mainwindow.hpp"
#include <QStatusBar>

void notifications::notify(QString title, QString body, QSystemTrayIcon::MessageIcon icon)
{
    MainWindow::inst()->tray->showMessage(title, body, icon, 5000);
    MainWindow::inst()->statusBar()->showMessage(title + ": " + body);
}
