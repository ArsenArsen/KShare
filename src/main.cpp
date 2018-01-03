#include "mainwindow.hpp"
#include <ui_mainwindow.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QScreen>
#include <QtGlobal>
#include <QFile>
#include <formatter.hpp>
#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}
#include <QListWidget>
#include <QTranslator>
#include <notifications.hpp>
#include <platformbackend.hpp>

bool verbose = false;

// I've experiments to run
// There is research to be done
// On the people who are
// still alive
bool stillAlive = true;

#define LOGACT(lvl) std::cout << lvl << stdMsg << "\n" << std::flush;
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
        break;
    case QtWarningMsg:
        LOGACT("WARN: ")
        break;
    case QtCriticalMsg:
        LOGACT("CRIT: ")
        break;
    case QtFatalMsg:
        LOGACT("FATAL: ")
        break;
    }
}

void loadTranslation(QString locale) {
    QFile resource(":/translations/" + locale + ".qm");
    if (!resource.exists()) return;
    resource.open(QIODevice::ReadOnly);

    QTranslator *translator = new QTranslator;
    QByteArray file = resource.readAll();
    QByteArray *permFile = new QByteArray;
    permFile->swap(file);
    translator->load((const unsigned char *)permFile->constData(), permFile->size());
    QApplication::installTranslator(translator);
}

int main(int argc, char *argv[]) {
    av_register_all();
    avdevice_register_all();
    qInstallMessageHandler(handler);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setApplicationName("KShare");
    a.setOrganizationName("ArsenArsen");
    a.setApplicationVersion("4.1");

    QString locale = QLocale::system().name();
    if (locale != "en_US") loadTranslation(locale);

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
    a.connect(&a, &QApplication::aboutToQuit, [] { stillAlive = false; });

    if (!parser.isSet(h)) w.show();
    return a.exec();
}
