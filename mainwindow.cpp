#include "mainwindow.hpp"
#include "screenshotter.hpp"
#include "screenshotutil.hpp"
#include "ui_mainwindow.h"
#include <QAction>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QListWidgetItem>
#include <QMenu>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTimer>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

MainWindow *MainWindow::instance;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    instance = this;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/icon.jpg"));
    tray = new QSystemTrayIcon(windowIcon(), this);
    tray->setToolTip("KShare");
    tray->setVisible(true);
    QMenu *menu = new QMenu(this);
    QAction *quit = new QAction("Quit", this);
    QAction *shtoggle = new QAction("Show/Hide", this);
    QAction *fullscreen = new QAction("Take fullscreen shot", this);
    QAction *area = new QAction("Take area shot", this);
    menu->addActions({ quit, shtoggle });
    menu->addSeparator();
    menu->addActions({ fullscreen, area });
    connect(quit, &QAction::triggered, this, &MainWindow::quit);
    connect(shtoggle, &QAction::triggered, this, &MainWindow::toggleVisible);
    connect(fullscreen, &QAction::triggered, this, &MainWindow::on_actionFullscreen_triggered);
    connect(area, &QAction::triggered, this, &MainWindow::on_actionArea_triggered);
    tray->setContextMenu(menu);

    ui->uploaderList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->uploaderList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Add items to uploader selection
    for (Uploader *u : UploaderSingleton::inst().uploaderList()) newUploader(u);
    connect(&UploaderSingleton::inst(), &UploaderSingleton::newUploader, this, &MainWindow::newUploader);

    // Set filename scheme
    if ((settings::settings().contains("fileFormat")))
        setScheme(settings::settings().value("fileFormat").toString());
    else
        setScheme("Screenshot %(yyyy-MM-dd HH:mm:ss)date");

    auto errors = UploaderSingleton::inst().errors();
    if (errors.length() == 1)
        statusBar()->showMessage(errors.at(0).what());
    else
        statusBar()->showMessage(QString("Errors visible in console (if present). Count: " + QString::number(errors.size())));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setScheme(QString scheme)
{
    ui->nameScheme->setText(scheme);
}

MainWindow *MainWindow::inst()
{
    return instance;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QTimer::singleShot(0, this, &MainWindow::hide);
}

void MainWindow::quit()
{
    QCoreApplication::quit();
}

void MainWindow::toggleVisible()
{
    this->setVisible(!this->isVisible());
}

void MainWindow::newUploader(Uploader *u)
{
    QListWidgetItem *item = new QListWidgetItem(u->name());
    item->setToolTip(u->description());
    ui->uploaderList->addItem(item);
    if (u->name() == UploaderSingleton::inst().selectedUploader()) item->setSelected(true);
}

void MainWindow::on_actionQuit_triggered()
{
    quit();
}

void MainWindow::on_actionFullscreen_triggered()
{
    QTimer::singleShot(0, &screenshotter::fullscreen);
}

void MainWindow::on_actionArea_triggered()
{
    QTimer::singleShot(0, &screenshotter::area);
}

void MainWindow::on_uploaderList_clicked(const QModelIndex &)
{
    QList<QListWidgetItem *> index = ui->uploaderList->selectedItems();
    if (index.size() == 1)
    {
        UploaderSingleton::inst().set(index.at(0)->text());
    }
}

void MainWindow::on_nameScheme_textEdited(const QString &arg1)
{
    settings::settings().setValue("fileFormat", arg1);
}
