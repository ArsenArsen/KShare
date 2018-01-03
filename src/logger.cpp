#include "logger.hpp"
#include "mainwindow.hpp"
#include "notifications.hpp"
#include <QDebug>
#include <iostream>
#include <stdlib.h>
#include <ui_mainwindow.h>

void logger::info(QString info) {
    qInfo() << info;
    if (MainWindow::inst() && MainWindow::inst()->valid()) {
        MainWindow::inst()->ui->logBox->addItem(info);
        notifications::notifyNolog(QObject::tr("KShare Message"), info, QSystemTrayIcon::Information);
    }
}

void logger::warn(QString info) {
    qWarning() << info;
    if (MainWindow::inst() && MainWindow::inst()->valid()) {
        MainWindow::inst()->ui->logBox->addItem(info);
        notifications::notifyNolog(QObject::tr("KShare Warning"), info, QSystemTrayIcon::Warning);
    }
}

void logger::error(QString info) {
    std::cerr << "ERROR: " << info.toStdString();
    if (MainWindow::inst() && MainWindow::inst()->valid()) {
        MainWindow::inst()->ui->logBox->addItem(info);
        notifications::notifyNolog(QObject::tr("KShare Error"), info, QSystemTrayIcon::Critical);
    }
}

void logger::fatal(QString info) {
    std::cerr << "FATAL: " << info.toStdString();
    if (MainWindow::inst() && MainWindow::inst()->valid()) {
        MainWindow::inst()->ui->logBox->addItem(info);
        notifications::notifyNolog(QObject::tr("KShare Fatal Error"), info, QSystemTrayIcon::Critical);
    }
}

void logger::abort(QString info) {
    std::cerr << "ABORT:" << info.toStdString();
    if (MainWindow::inst() && MainWindow::inst()->valid()) {
        MainWindow::inst()->ui->logBox->addItem(info);
        notifications::notifyNolog(QObject::tr("KShare Even More Fatal Error"), info, QSystemTrayIcon::Critical);
    }
    ::abort();
}
