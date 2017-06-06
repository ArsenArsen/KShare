#include "mainwindow.hpp"
#include "screenshotter.hpp"
#include "screenshotutil.hpp"
#include "ui_mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTimer>
#include <colorpicker/colorpickerscene.hpp>
#include <functional>
#include <hotkeying.hpp>
#include <recording/recordingformats.hpp>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

MainWindow *MainWindow::instance;

void addHotkeyItem(QString text, QString name, std::function<void()> func, QString def = QString()) {
    QListWidgetItem *item = new QListWidgetItem(text, MainWindow::inst()->ui->hotkeys);
    item->setData(Qt::UserRole + 1, name);
    MainWindow::inst()->fncs.insert(name, func);
    hotkeying::load(name, func, def);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    instance = this;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/icon.png"));
    tray = new QSystemTrayIcon(windowIcon(), this);
    tray->setToolTip("KShare");
    tray->setVisible(true);
    QMenu *menu = new QMenu(this);
    QAction *quit = new QAction("Quit", this);
    QAction *shtoggle = new QAction("Show/Hide", this);
    QAction *fullscreen = new QAction("Take fullscreen shot", this);
    QAction *area = new QAction("Take area shot", this);
    QAction *picker = new QAction("Show color picker", this);
    menu->addActions({ quit, shtoggle, picker });
    menu->addSeparator();
    menu->addActions({ fullscreen, area });
    connect(quit, &QAction::triggered, this, &MainWindow::quit);
    connect(shtoggle, &QAction::triggered, this, &MainWindow::toggleVisible);
    connect(picker, &QAction::triggered, [] { ColorPickerScene::showPicker(); });
    connect(tray, &QSystemTrayIcon::messageClicked, this, &MainWindow::toggleVisible);
    connect(tray, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) toggleVisible();
    });
    connect(fullscreen, &QAction::triggered, this, [] { screenshotter::fullscreenDelayed(); });
    connect(area, &QAction::triggered, this, [] { screenshotter::areaDelayed(); });
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

    // Set delay
    if ((settings::settings().contains("delay")))
        ui->delay->setValue(settings::settings().value("delay").toDouble());
    else
        ui->delay->setValue(0.25);

    ui->hotkeys->setSelectionMode(QListWidget::SingleSelection);

    addHotkeyItem("Fullscreen image", "fullscreen", [] { screenshotter::fullscreen(); });
    addHotkeyItem("Area image", "area", [] { screenshotter::area(); });
    addHotkeyItem("Color picker", "picker", [] { ColorPickerScene::showPicker(); });
    addHotkeyItem("Stop Recording", "recordingstop", [&] { controller->end(); });
    addHotkeyItem("Start Recording", "recordingstart", [&] {
        auto f = static_cast<RecordingFormats::Format>(
        settings::settings().value("recording/format", (int)RecordingFormats::None).toInt());
        if (f == RecordingFormats::None) return;
        RecordingContext *ctx = new RecordingContext;
        RecordingFormats *format = new RecordingFormats(f);
        ctx->consumer = format->getConsumer();
        ctx->finalizer = format->getFinalizer();
        ctx->validator = format->getValidator();
        ctx->format = format->getFormat();
        controller->start(ctx);
    });

    ui->quickMode->setChecked(settings::settings().value("quickMode", false).toBool());
    ui->hideToTray->setChecked(settings::settings().value("hideOnClose", true).toBool());
    ui->captureCursor->setChecked(settings::settings().value("captureCursor", true).toBool());

    for (int i = 0; i < RecordingFormats::None; i++) {
        ui->formatBox->addItem(RecordingFormats::getExt(static_cast<RecordingFormats::Format>(i)));
    }
    ui->formatBox->addItem("None");
    ui->formatBox->setCurrentIndex(settings::settings().value("recording/format", (int)RecordingFormats::None).toInt());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setScheme(QString scheme) {
    ui->nameScheme->setText(scheme);
}

QDoubleSpinBox *MainWindow::delay() {
    return ui->delay;
}

MainWindow *MainWindow::inst() {
    return instance;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (settings::settings().value("hideOnClose", true).toBool()) {
        event->ignore();
        QTimer::singleShot(0, this, &MainWindow::hide);
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

void MainWindow::newUploader(Uploader *u) {
    QListWidgetItem *item = new QListWidgetItem(u->name());
    item->setToolTip(u->description());
    ui->uploaderList->addItem(item);
    if (u->name() == UploaderSingleton::inst().selectedUploader()) item->setSelected(true);
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

void MainWindow::on_uploaderList_clicked(const QModelIndex &) {
    QList<QListWidgetItem *> index = ui->uploaderList->selectedItems();
    if (index.size() == 1) {
        UploaderSingleton::inst().set(index.at(0)->text());
    }
}

void MainWindow::on_nameScheme_textEdited(const QString &arg1) {
    settings::settings().setValue("fileFormat", arg1);
}

void MainWindow::on_delay_valueChanged(double arg1) {
    settings::settings().setValue("delay", arg1);
}

void MainWindow::on_hotkeys_doubleClicked(const QModelIndex &) {
    if (ui->hotkeys->selectedItems().length() == 1) {
        QListWidgetItem *i = ui->hotkeys->selectedItems().at(0);
        QString str = i->data(Qt::UserRole + 1).toString();
        bool ok;
        QString seq = QInputDialog::getText(ui->centralWidget, "Hotkey Input", "Insert hotkey:", QLineEdit::Normal,
                                            hotkeying::sequence(str), &ok);
        if (ok && hotkeying::valid(seq)) hotkeying::hotkey(str, QKeySequence(seq), fncs.value(str));
    }
}

void MainWindow::on_settingsButton_clicked() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/KShare"));
}

void MainWindow::on_quickMode_clicked(bool checked) {
    settings::settings().setValue("quickMode", checked);
}

void MainWindow::on_hideToTray_clicked(bool checked) {
    settings::settings().setValue("hideOnClose", checked);
}

void MainWindow::on_actionColor_Picker_triggered() {
    ColorPickerScene::showPicker();
}

void MainWindow::on_captureCursor_clicked(bool checked) {
    settings::settings().setValue("captureCursor", checked);
}

void MainWindow::on_formatBox_currentIndexChanged(int index) {
    settings::settings().setValue("recording/format", index);
}
