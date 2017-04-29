#include "mainwindow.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QtGlobal>
#include <stdio.h>

bool verbose = false;

void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        if (verbose)
            fprintf(stdout, "DEBUG %s:%s(%d): %s\n", context.file, context.function, context.line, localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stdout, "INFO %s:%s(%d): %s\n", context.file, context.function, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stdout, "WARN %s:%s(%d): %s\n", context.file, context.function, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stdout, "CRIT %s:%s(%d): %s\n", context.file, context.function, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stdout, "FATAL %s:%s(%d): %s\n", context.file, context.function, context.line, localMsg.constData());
        break;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(handler);
    QApplication a(argc, argv);
    a.setApplicationName("KShare");
    a.setOrganizationName("ArsenArsen");
    a.setApplicationVersion("1.1");

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption h({ "b", "background" }, "Does not show the main window, starts in tray.");
    QCommandLineOption v({ "v", "verbose" }, "Enables QtDebugMsg outputs");
    parser.addOption(h);
    parser.addOption(v);
    parser.process(a);
    verbose = parser.isSet(v);

    MainWindow w;
    w.show();
    if (parser.isSet(h)) w.hide();
    return a.exec();
}
