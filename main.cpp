#include "mainwindow.hpp"
#include "screenshotutil.hpp"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QScreen>
#include <QtGlobal>
#include <formatter.hpp>
#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include <QListWidget>
#include <notifications.hpp>
#include <platformbackend.hpp>
#include <worker/worker.hpp>

bool verbose = false;

// I've experiments to run
// There is research to be done
// On the people who are
// still alive
bool stillAlive = true;

#define LOGACT(lvl)                                                                                                    \
    std::cout << lvl << stdMsg << "\n";                                                                                \
    if (stillAlive && MainWindow::inst() && MainWindow::inst()->valid()) {                                             \
        MainWindow::inst()->ui->logBox->addItem(lvl + msg);                                                            \
    }

void handler(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    if (!verbose && msg.startsWith("QPixmap::fromWinHBITMAP")) return;
    std::string stdMsg = msg.toStdString();
    switch (type) {
    case QtDebugMsg:
        if (verbose) {
            LOGACT("DEBUG: ")
        }
        break;
    case QtInfoMsg:
        LOGACT("INFO: ")
        if (stillAlive) notifications::notifyNolog("KShare", msg);
        break;
    case QtWarningMsg:
        LOGACT("WARN: ")
        if (stillAlive) notifications::notifyNolog("KShare Warning", msg, QSystemTrayIcon::Warning);
        break;
    case QtCriticalMsg:
        LOGACT("CRIT: ")
        if (stillAlive) notifications::notifyNolog("KShare Critical Error", msg, QSystemTrayIcon::Critical);
        break;
    case QtFatalMsg:
        LOGACT("FATAL: ")
        if (stillAlive) notifications::notifyNolog("KShare Fatal Error", msg, QSystemTrayIcon::Critical);
        break;
    }
}

int main(int argc, char *argv[]) {
    av_register_all();
    qInstallMessageHandler(handler);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setApplicationName("KShare");
    a.setOrganizationName("ArsenArsen");
    a.setApplicationVersion("4.1");

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption h({ "b", "background" }, "Does not show the main window, starts in tray.");
    QCommandLineOption v({ "v", "verbose" }, "Enables QtDebugMsg outputs");
    QCommandLineOption ver({ "ver", "version" }, "Prints KShare version");
    parser.addOption(h);
    parser.addOption(v);
    parser.addOption(ver);
    parser.process(a);

    if (parser.isSet(ver)) {
        printf("%s %s\n", a.applicationName().toLocal8Bit().constData(), a.applicationVersion().toLocal8Bit().constData());
        return 0;
    }

    verbose = parser.isSet(v);
    MainWindow w;
    Worker::init();
    a.connect(&a, &QApplication::aboutToQuit, Worker::end);
    a.connect(&a, &QApplication::aboutToQuit, [] { stillAlive = false; });
    screenshotutil::renderText("DICKS").save("/home/arsen/test.png");
    if (!parser.isSet(h)) w.show();
    return a.exec();
}
