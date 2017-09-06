#include "encodersettingsdialog.hpp"
#include "encodersettings.hpp"
#include "ui_encodersettingsdialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QSpinBox>

EncoderSettingsDialog::EncoderSettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EncoderSettingsDialog) {
    ui->setupUi(this);
    setWindowTitle(tr("KShare Encoder Settings"));
    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&] {
        EncoderSettings::inst().setbitrate(ui->bitrate->value());
        EncoderSettings::inst().seth264Crf(ui->crf->value());
        EncoderSettings::inst().seth264Profile(ui->profileBox->currentText());
        EncoderSettings::inst().setvp9Lossless(ui->vp9Lossless->isChecked());
        EncoderSettings::inst().setgopSize(ui->gopSize->value());
        EncoderSettings::inst().setimageQuality(ui->defaultImageQuality->isChecked() ? -1 : ui->imageQuality->value());
    });
}

EncoderSettingsDialog::~EncoderSettingsDialog() {
    delete ui;
}

void EncoderSettingsDialog::on_defaultImageQuality_clicked(bool checked) {
    ui->imageQuality->setEnabled(!checked);
}
