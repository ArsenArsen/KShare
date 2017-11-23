#include "mainwindow.hpp"
#include "aboutbox.hpp"
#include "screenshotter.hpp"
#include "settingsdialog.hpp"
#include "ui_mainwindow.h"
#include "utils.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <colorpicker/colorpickerscene.hpp>
#include <formats.hpp>
#include <hotkeying.hpp>
#include <logs/historydialog.hpp>
#include <platformbackend.hpp>
#include <recording/recordingformats.hpp>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

MainWindow *MainWindow::instance;

void MainWindow::rec() {
    if (controller->isRunning()) return;
    auto f = static_cast<formats::Recording>(
    settings::settings().value("recording/format", static_cast<int>(formats::Recording::None)).toInt());
    if (f >= formats::Recording::None) {
        qWarning() << tr("Recording format not set in settings. Aborting.");
        return;
    }
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

#define ACTION(english, menu)                                                                                          \
    [&]() -> QAction * {                                                                                               \
        QAction *a = menu->addAction(english);                                                                         \
        return a;                                                                                                      \
    }()

void addHotkey(QString name, std::function<void()> action) {
    hotkeying::load(name, action);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    instance = this;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/icon.png"));
    tray = new QSystemTrayIcon(windowIcon(), this);
    tray->setToolTip("KShare");
    tray->setVisible(true);
    menu = new QMenu(this);
    QAction *quit = ACTION(tr("Quit"), menu);
    QAction *shtoggle = ACTION(tr("Show/Hide"), menu);
    QAction *fullscreen = ACTION(tr("Take fullscreen shot"), menu);
    QAction *area = ACTION(tr("Take area shot"), menu);

#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    QAction *active = ACTION(tr("Screenshot active window"), menu);
    connect(active, &QAction::triggered, this, [] { screenshotter::activeDelayed(); });
#endif
    QAction *picker = ACTION(tr("Show color picker"), menu);
    QAction *rec = ACTION(tr("Record screen"), menu);
    QAction *recoff = ACTION(tr("Stop recording"), menu);
    QAction *recabort = ACTION(tr("Abort recording"), menu);
    menu->addActions({ quit, shtoggle, picker });
    menu->addSeparator();
    menu->addActions({ fullscreen, area });
#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    menu->addAction(active);
#endif
    menu->addSeparator();
    menu->addActions({ rec, recoff, recabort });
    connect(quit, &QAction::triggered, this, &MainWindow::quit);
    connect(shtoggle, &QAction::triggered, this, &MainWindow::toggleVisible);
    connect(picker, &QAction::triggered, [] { ColorPickerScene::showPicker(); });
    connect(tray, &QSystemTrayIcon::messageClicked, this, &QWidget::show);
    connect(tray, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) toggleVisible();
    });
    connect(fullscreen, &QAction::triggered, this, [] { screenshotter::fullscreenDelayed(); });
    connect(area, &QAction::triggered, this, [] { screenshotter::areaDelayed(); });
    connect(rec, &QAction::triggered, this, &MainWindow::rec);
    connect(recoff, &QAction::triggered, controller, &RecordingController::end);
    connect(recabort, &QAction::triggered, controller, &RecordingController::abort);
    connect(ui->settings, &QPushButton::clicked, this, &MainWindow::on_actionSettings_triggered);

    tray->setContextMenu(menu);

    addHotkey("fullscreen", [] { screenshotter::fullscreen(); });
    addHotkey("area", [] { screenshotter::area(); });
    addHotkey("active", [] { screenshotter::active(); });
    addHotkey("picker", [] { ColorPickerScene::showPicker(); });
    addHotkey("recordingstop", [&] { controller->end(); });
    addHotkey("recordingabort", [&] { controller->abort(); });
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

void MainWindow::on_actionAbort_triggered() {
    controller->abort();
}

void MainWindow::on_history_clicked() {
    HistoryDialog *dialog = new HistoryDialog;
    dialog->show();
}
