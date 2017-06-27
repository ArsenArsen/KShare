#include "mainwindow.hpp"
#include "aboutbox.hpp"
#include "screenshotter.hpp"
#include "screenshotutil.hpp"
#include "settingsdialog.hpp"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <colorpicker/colorpickerscene.hpp>
#include <formats.hpp>
#include <hotkeying.hpp>
#include <platformbackend.hpp>
#include <recording/recordingformats.hpp>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

MainWindow *MainWindow::instance;

void MainWindow::rec() {
    if (controller->isRunning()) return;
    auto f
    = static_cast<formats::Recording>(settings::settings().value("recording/format", (int)formats::Recording::None).toInt());
    if (f >= formats::Recording::None) return;
    RecordingContext *ctx = new RecordingContext;
    RecordingFormats *format = new RecordingFormats(f);
    ctx->consumer = format->getConsumer();
    ctx->finalizer = format->getFinalizer();
    ctx->validator = format->getValidator();
    ctx->format = format->getFormat();
    ctx->postUploadTask = format->getPostUploadTask();
    ctx->anotherFormat = format->getAnotherFormat();
    controller->start(ctx);
}

void addHotkey(QString name, std::function<void()> action) {
    hotkeying::load(name, action);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    instance = this;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/icon.svg"));
    tray = new QSystemTrayIcon(windowIcon(), this);
    tray->setToolTip("KShare");
    tray->setVisible(true);
    QMenu *menu = new QMenu(this);
    QAction *quit = new QAction("Quit", this);
    QAction *shtoggle = new QAction("Show/Hide", this);
    QAction *fullscreen = new QAction("Take fullscreen shot", this);
    QAction *area = new QAction("Take area shot", this);

#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    QAction *active = new QAction("Screenshot active window", this);
    connect(active, &QAction::triggered, this, [] { screenshotter::activeDelayed(); });
#endif
    QAction *picker = new QAction("Show color picker", this);
    QAction *rec = new QAction("Record screen", this);
    QAction *recoff = new QAction("Stop recording", this);
    menu->addActions({ quit, shtoggle, picker });
    menu->addSeparator();
    menu->addActions({ fullscreen, area, active });
#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    menu->addAction(area);
#endif
    menu->addSeparator();
    menu->addActions({ rec, recoff });
    connect(quit, &QAction::triggered, this, &MainWindow::quit);
    connect(shtoggle, &QAction::triggered, this, &MainWindow::toggleVisible);
    connect(picker, &QAction::triggered, [] { ColorPickerScene::showPicker(); });
    connect(tray, &QSystemTrayIcon::messageClicked, this, &MainWindow::toggleVisible);
    connect(tray, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) toggleVisible();
    });
    connect(fullscreen, &QAction::triggered, this, [] { screenshotter::fullscreenDelayed(); });
    connect(area, &QAction::triggered, this, [] { screenshotter::areaDelayed(); });
    connect(rec, &QAction::triggered, this, &MainWindow::rec);
    connect(recoff, &QAction::triggered, [this] { controller->end(); });
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::on_actionSettings_triggered);

    tray->setContextMenu(menu);

    addHotkey("fullscreen", [] { screenshotter::fullscreen(); });
    addHotkey("area", [] { screenshotter::area(); });
    addHotkey("active", [] { screenshotter::active(); });
    addHotkey("picker", [] { ColorPickerScene::showPicker(); });
    addHotkey("recordingstop", [&] { controller->end(); });
    addHotkey("recordingstart", [&] { this->rec(); });

    auto errors = UploaderSingleton::inst().errors();
    for (auto err : errors) ui->logBox->addItem(QString("ERROR: ") + err.what());
    setWindowTitle("KShare v" + QApplication::applicationVersion());
    val = true;
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::valid() {
    return val;
}

MainWindow *MainWindow::inst() {
    return instance;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (settings::settings().value("hideOnClose", true).toBool()) {
        event->ignore();
        hide();
    } else
        QApplication::exit(0);
}

void MainWindow::quit() {
    QCoreApplication::quit();
}

void MainWindow::toggleVisible() {
    this->setVisible(!this->isVisible());
    if (this->isVisible()) {
        this->raise();                          // that didn't work
        this->setWindowState(Qt::WindowActive); // maybe that works
        this->activateWindow();                 // maybe that works
    }
}

void MainWindow::on_actionQuit_triggered() {
    quit();
}

void MainWindow::on_actionFullscreen_triggered() {
    screenshotter::fullscreenDelayed();
}

void MainWindow::on_actionArea_triggered() {
    screenshotter::areaDelayed();
}

void MainWindow::on_actionStart_triggered() {
    rec();
}

void MainWindow::on_actionStop_triggered() {
    controller->end();
}

void MainWindow::on_actionColor_Picker_triggered() {
    ColorPickerScene::showPicker();
}

void MainWindow::on_actionSettings_triggered() {
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::on_actionAbout_triggered() {
    AboutBox *box = new AboutBox(this);
    box->setAttribute(Qt::WA_DeleteOnClose);
    box->show();
}

void MainWindow::on_actionActive_window_triggered() {
    screenshotter::activeDelayed();
}
