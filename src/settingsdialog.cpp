#include "settingsdialog.hpp"
#include "filenamevalidator.hpp"
#include "hotkeyinputdialog.hpp"
#include "mainwindow.hpp"
#include "ui_settingsdialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QInputDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStandardPaths>
#include <colorpicker/colorpickerscene.hpp>
#include <formats.hpp>
#include <hotkeying.hpp>
#include <platformbackend.hpp>
#include <recording/encoders/encodersettingsdialog.hpp>
#include <screenshotter.hpp>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

static QMap<QString, std::function<void()>> fncs;

void addHotkeyItem(QListWidget *hotkeys, QString text, QString name, std::function<void()> func, QString def = QString()) {
    QListWidgetItem *item = new QListWidgetItem(text, hotkeys);
    item->setData(Qt::UserRole + 1, name);
    hotkeys->addItem(item);
    fncs.insert(name, func);
    hotkeying::load(name, func, def);
}

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    ui->uploaderList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->uploaderList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Add items to uploader selection
    connect(&UploaderSingleton::inst(), &UploaderSingleton::newUploader, this, &SettingsDialog::newUploader);
    connect(&UploaderSingleton::inst(), &UploaderSingleton::uploaderChanged, this, &SettingsDialog::uploaderChanged);
    for (Uploader *u : UploaderSingleton::inst().uploaderList()) newUploader(u);

    // Set filename scheme
    setScheme(settings::settings().value("fileFormat", "Screenshot %(yyyy-MM-dd HH-mm-ss)date.%ext").toString());

    // Set delay
    if ((settings::settings().contains("delay")))
        ui->delay->setValue(settings::settings().value("delay").toDouble());
    else
        ui->delay->setValue(0.25);

    ui->hotkeys->setSelectionMode(QListWidget::SingleSelection);

    addHotkeyItem(ui->hotkeys, tr("Fullscreen image"), "fullscreen", [] { screenshotter::fullscreen(); });
    addHotkeyItem(ui->hotkeys, tr("Area image"), "area", [] { screenshotter::area(); });
#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    addHotkeyItem(ui->hotkeys, tr("Active window"), "active", [&] { screenshotter::active(); });
#endif
    addHotkeyItem(ui->hotkeys, tr("Color picker"), "picker", [] { ColorPickerScene::showPicker(); });
    addHotkeyItem(ui->hotkeys, tr("Stop Recording"), "recordingstop", [&] { MainWindow::inst()->controller->stop(); });
    addHotkeyItem(ui->hotkeys, tr("Start Recording"), "recordingstart", [&] { MainWindow::inst()->rec(); });

    ui->quickMode->setChecked(settings::settings().value("quickMode", false).toBool());
    ui->hideToTray->setChecked(settings::settings().value("hideOnClose", true).toBool());
    ui->captureCursor->setChecked(settings::settings().value("captureCursor", true).toBool());
    ui->saveLocation->setCurrentIndex(settings::settings().value("saveLocation", 1).toInt());
    for (int i = 0; i < (int)formats::Recording::None; i++) {
        ui->formatBox->addItem(formats::recordingFormatName(static_cast<formats::Recording>(i)));
    }

    for (int i = 0; i < (int)formats::Normal::None; i++) {
        ui->imageFormatBox->addItem(formats::normalFormatName(static_cast<formats::Normal>(i)));
    }

    ui->formatBox->addItem("None");
    ui->formatBox->setCurrentIndex(settings::settings().value("recording/format", (int)formats::Recording::None).toInt());
    ui->cropX->setValue(settings::settings().value("cropx", 0).toInt());
    ui->cropY->setValue(settings::settings().value("cropy", 0).toInt());
    setWindowTitle("Settings");
    ui->nameScheme->setValidator(new FilenameValidator(ui->nameScheme));
#ifndef PLATFORM_CAPABILITY_CURSOR
    ui->captureCursor->setEnabled(false);
    ui->captureCursor->setText(tr("Capture cursor (disabled: implementation missing)"));
#endif
    ui->fpsMax->setValue(settings::settings().value("recording/framerate", 30).toInt());
    ui->focusedCapture->setText(settings::settings().value("command/activeCommand", "").toString());
    ui->fullscreenCapture->setText(settings::settings().value("command/fullscreenCommand", "").toString());
}

void SettingsDialog::setScheme(QString scheme) {
    ui->nameScheme->setText(scheme);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::newUploader(Uploader *u) {
    QListWidgetItem *uploader = new QListWidgetItem(u->name(), ui->uploaderList);
    uploader->setToolTip(u->description());
    ui->uploaderList->addItem(uploader);
    if (UploaderSingleton::inst().currentUploader() == u->name()) ui->uploaderList->setCurrentItem(uploader);
}

void SettingsDialog::uploaderChanged(QString newName) {
    for (auto item : ui->uploaderList->findItems(newName, Qt::MatchCaseSensitive))
        ui->uploaderList->setCurrentItem(item);
}

void SettingsDialog::on_uploaderList_clicked(const QModelIndex &) {
    UploaderSingleton::inst().set(ui->uploaderList->currentItem()->text());
}

void SettingsDialog::on_uploaderList_doubleClicked(const QModelIndex &) {
    UploaderSingleton::inst().showSettings();
}

void SettingsDialog::on_nameScheme_textChanged(const QString &arg1) {
    settings::settings().setValue("fileFormat", arg1);
}

void SettingsDialog::on_delay_valueChanged(double arg1) {
    settings::settings().setValue("delay", arg1);
}

void SettingsDialog::on_hotkeys_doubleClicked(const QModelIndex &) {
    QListWidgetItem *i = ui->hotkeys->currentItem();
    QString str = i->data(Qt::UserRole + 1).toString();
    HotkeyInputDialog *hotkey = new HotkeyInputDialog(str, hotkeying::sequence(str), this);
    connect(hotkey, &HotkeyInputDialog::sequenceSelected,
            [&](QKeySequence seq, QString name) { hotkeying::hotkey(name, seq, fncs.value(name)); });
    hotkey->show();
}

void SettingsDialog::on_settingsButton_clicked() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/KShare"));
}

void SettingsDialog::on_quickMode_clicked(bool checked) {
    settings::settings().setValue("quickMode", checked);
}

void SettingsDialog::on_hideToTray_clicked(bool checked) {
    settings::settings().setValue("hideOnClose", checked);
}

void SettingsDialog::on_captureCursor_clicked(bool checked) {
    settings::settings().setValue("captureCursor", checked);
}

void SettingsDialog::on_formatBox_currentIndexChanged(int index) {
    if (isVisible()) settings::settings().setValue("recording/format", index);
}

void SettingsDialog::on_imageFormatBox_currentIndexChanged(int index) {
    if (isVisible()) settings::settings().setValue("imageformat", index);
}

void SettingsDialog::on_pushButton_clicked() {
    auto a = new EncoderSettingsDialog();
    a->setAttribute(Qt::WA_DeleteOnClose);
    a->show();
}

void SettingsDialog::on_saveLocation_currentIndexChanged(int index) {
    settings::settings().setValue("saveLocation", index);
}

void SettingsDialog::on_cropX_valueChanged(int arg1) {
    settings::settings().setValue("cropx", arg1);
}

void SettingsDialog::on_cropY_valueChanged(int arg1) {
    settings::settings().setValue("cropy", arg1);
}

void SettingsDialog::on_focusedCapture_textChanged(QString arg1) {
    settings::settings().setValue("command/activeCommand", arg1);
}

void SettingsDialog::on_fullscreenCapture_textChanged(QString arg1) {
    settings::settings().setValue("command/fullscreenCommand", arg1);
}

void SettingsDialog::on_fpsMax_valueChanged(int arg1) {
    settings::settings().setValue("recording/framerate", arg1);
}
