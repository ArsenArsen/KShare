#include "mainwindow.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <QtGlobal>
#include <stdio.h>

bool verbose = false;

void handler(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        if (verbose) fprintf(stderr, "DEBUG: %s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "INFO: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "WARN: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRIT: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL: %s\n", localMsg.constData());
        break;
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(handler);
    QApplication a(argc, argv);
    a.setApplicationName("KShare");
    a.setOrganizationName("ArsenArsen");
    a.setApplicationVersion("3.0");

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
    w.show();
    QTimer::singleShot(0, [&] {
        if (parser.isSet(h)) w.hide();
    });
    return a.exec();
}
