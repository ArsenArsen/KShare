#include "settingsdialog.hpp"
#include "hotkeyinputdialog.hpp"
#include "mainwindow.hpp"
#include "ui_settingsdialog.h"

#include <QCheckBox>
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

QMap<QString, std::function<void()>> fncs;

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
    for (Uploader *u : UploaderSingleton::inst().uploaderList()) newUploader(u);

    // Set filename scheme
    setScheme(settings::settings().value("fileFormat", "Screenshot %(yyyy-MM-dd HH:mm:ss)date.ext").toString());


    // Set delay
    if ((settings::settings().contains("delay")))
        ui->delay->setValue(settings::settings().value("delay").toDouble());
    else
        ui->delay->setValue(0.25);

    ui->hotkeys->setSelectionMode(QListWidget::SingleSelection);

    addHotkeyItem(ui->hotkeys, "Fullscreen image", "fullscreen", [] { screenshotter::fullscreen(); });
    addHotkeyItem(ui->hotkeys, "Area image", "area", [] { screenshotter::area(); });
#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    addHotkeyItem(ui->hotkeys, "Active window", "active", [&] { screenshotter::active(); });
#endif
    addHotkeyItem(ui->hotkeys, "Color picker", "picker", [] { ColorPickerScene::showPicker(); });
    addHotkeyItem(ui->hotkeys, "Stop Recording", "recordingstop", [&] { MainWindow::inst()->controller->end(); });
    addHotkeyItem(ui->hotkeys, "Start Recording", "recordingstart", [&] { MainWindow::inst()->rec(); });

    ui->quickMode->setChecked(settings::settings().value("quickMode", false).toBool());
    ui->hideToTray->setChecked(settings::settings().value("hideOnClose", true).toBool());
    ui->captureCursor->setChecked(settings::settings().value("captureCursor", true).toBool());
    for (int i = 0; i < (int)formats::Recording::None; i++) {
        ui->formatBox->addItem(formats::recordingFormatName(static_cast<formats::Recording>(i)));
    }

    for (int i = 0; i < (int)formats::Normal::None; i++) {
        ui->imageFormatBox->addItem(formats::normalFormatName(static_cast<formats::Normal>(i)));
    }

    ui->formatBox->addItem("None");
    ui->formatBox->setCurrentIndex(settings::settings().value("recording/format", (int)formats::Recording::None).toInt());
    setWindowTitle("Settings");
#ifndef PLATFORM_CAPABILITY_CURSOR
    ui->captureCursor->setEnabled(false);
    ui->captureCursor->setText("Capture cursor (disabled: implementation missing)");
#endif
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

void SettingsDialog::on_uploaderList_clicked(const QModelIndex &) {
    QList<QListWidgetItem *> index = ui->uploaderList->selectedItems();
    if (index.size() == 1) {
        UploaderSingleton::inst().set(index.at(0)->text());
    }
}

void SettingsDialog::on_nameScheme_textChanged(const QString &arg1) {
    settings::settings().setValue("fileFormat", arg1);
}

void SettingsDialog::on_delay_valueChanged(double arg1) {
    settings::settings().setValue("delay", arg1);
}

void SettingsDialog::on_hotkeys_doubleClicked(const QModelIndex &) {
    if (ui->hotkeys->selectedItems().length() == 1) {
        QListWidgetItem *i = ui->hotkeys->selectedItems().at(0);
        QString str = i->data(Qt::UserRole + 1).toString();
        HotkeyInputDialog *hotkey = new HotkeyInputDialog(str, hotkeying::sequence(str), this);
        connect(hotkey, &HotkeyInputDialog::sequenceSelected,
                [&](QKeySequence seq, QString name) { hotkeying::hotkey(name, seq, fncs.value(name)); });
        hotkey->show();
    }
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
