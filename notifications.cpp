#include "notifications.hpp"

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QStatusBar>

void notifications::notify(QString title, QString body, QSystemTrayIcon::MessageIcon icon) {
    if (!MainWindow::inst() || !MainWindow::inst()->valid()) return;
    notifyNolog(title, body, icon);
    MainWindow::inst()->ui->logBox->addItem(title + ": " + body);
}

void notifications::notifyNolog(QString title, QString body, QSystemTrayIcon::MessageIcon icon) {
    if (!MainWindow::inst()) return;
    MainWindow::inst()->tray->showMessage(title, body, icon, 5000);
    MainWindow::inst()->statusBar()->showMessage(title + ": " + body);
}
