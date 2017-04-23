#include "mainwindow.hpp"
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <cropeditor/cropeditor.hpp>
#include <screenshotutil.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
