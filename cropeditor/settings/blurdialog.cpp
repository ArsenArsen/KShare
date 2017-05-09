#include "blurdialog.hpp"
#include "ui_blurdialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSlider>

BlurDialog::BlurDialog(QGraphicsBlurEffect *e, QWidget *parent) : QDialog(parent), ui(new Ui::BlurDialog) {
    effect = e;
    ui->setupUi(this);
    ui->animated->setChecked(effect->blurHints().testFlag(QGraphicsBlurEffect::AnimationHint));
    ui->performance->setChecked(effect->blurHints().testFlag(QGraphicsBlurEffect::PerformanceHint));
    ui->quality->setChecked(effect->blurHints().testFlag(QGraphicsBlurEffect::QualityHint));
    ui->radSlider->setValue(effect->blurRadius() * 100);
    ui->radSpinner->setValue(effect->blurRadius());
    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&] {
        QFlags<QGraphicsBlurEffect::BlurHint> hints;
        hints.setFlag(QGraphicsBlurEffect::AnimationHint, ui->animated->isChecked());
        hints.setFlag(QGraphicsBlurEffect::PerformanceHint, ui->performance->isChecked());
        hints.setFlag(QGraphicsBlurEffect::QualityHint, ui->quality->isChecked());
        effect->setBlurHints(hints);
        effect->setBlurRadius(ui->radSpinner->value());
        setResult(QDialog::Accepted);
        close();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [&] {
        setResult(QDialog::Rejected);
        close();
    });
}

BlurDialog::~BlurDialog() {
    delete ui;
}

void BlurDialog::on_radSpinner_valueChanged(double arg1) {
    ui->radSlider->setValue(arg1 * 100);
}

void BlurDialog::on_radSlider_sliderMoved(int position) {
    ui->radSpinner->setValue(position / 100.);
}
