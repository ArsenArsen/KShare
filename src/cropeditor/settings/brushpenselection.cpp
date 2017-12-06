#include "brushpenselection.hpp"
#include "ui_brushpenselection.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGraphicsBlurEffect>
#include <QInputDialog>
#include <QSlider>
#include <QTimer>
#include <cropeditor/cropview.hpp>
#include <settings.hpp>

BrushPenSelection::BrushPenSelection(CropScene *scene) : QDialog(), ui(new Ui::BrushPenSelection) {
    ui->setupUi(this);

    ui->animated->setChecked(settings::settings().value("blur/animatedHint", false).toBool());
    ui->performance->setChecked(settings::settings().value("blur/performanceHint", true).toBool());
    ui->quality->setChecked(settings::settings().value("blur/qualityHint", false).toBool());
    ui->radSlider->setValue(settings::settings().value("blur/radius", 5.).toDouble() * 100);
    ui->radSpinner->setValue(settings::settings().value("blur/radius", 5.).toDouble());

    ui->cosmetic->setChecked(scene->pen().isCosmetic());
    ui->widthSlider->setValue(scene->pen().width());
    ui->widthSpinner->setValue(scene->pen().widthF());
    ui->brushStyle->setCurrentIndex(settings::settings().value("brushStyle", 1).toInt());
    ui->pathItemHasBrush->setChecked(settings::settings().value("brushPath", false).toBool());

    ui->arroww->setValue(settings::settings().value("arrow/width", 20).toDouble());
    ui->arrowh->setValue(settings::settings().value("arrow/height", 10).toDouble());

    this->setFocus();
    pen = scene->pen().color();
    ui->penAlphaSlider->setValue(pen.alpha());
    ui->penAlphaSpin->setValue(pen.alpha());
    brush = scene->brush().color();
    ui->alphaSlider->setValue(brush.alpha());
    ui->alphaSpin->setValue(brush.alpha());

    setWindowTitle(tr("Crop editor settings"));
    this->scene = scene;
}

BrushPenSelection::~BrushPenSelection() {
    delete ui;
}

void BrushPenSelection::on_penColor_clicked(bool) {
    pen = QColorDialog::getColor(pen, this, tr("Pen Color"));
    pen.setAlpha(ui->penAlphaSpin->value());
}

void BrushPenSelection::on_brushColor_clicked(bool) {
    brush = QColorDialog::getColor(brush, this, tr("Brush Color"));
    brush.setAlpha(ui->alphaSpin->value());
}

void BrushPenSelection::on_buttonBox_accepted() {
    scene->pen().setColor(pen);
    scene->pen().setCosmetic(ui->cosmetic->isChecked());
    scene->pen().setWidthF(ui->widthSpinner->value());
    scene->brush().setColor(brush);
    scene->brush().setStyle((Qt::BrushStyle)ui->brushStyle->currentIndex());

    settings::settings().setValue("penColor", scene->pen().color());
    settings::settings().setValue("penCosmetic", scene->pen().isCosmetic());
    settings::settings().setValue("penWidth", scene->pen().widthF());
    settings::settings().setValue("brushColor", scene->brush().color());
    settings::settings().setValue("gridEnabled", scene->grid());
    settings::settings().setValue("brushStyle", (int)scene->brush().style());
    settings::settings().setValue("brushPath", ui->pathItemHasBrush->isChecked());
    settings::settings().setValue("blur/radius", ui->radSpinner->value());
    settings::settings().setValue("blur/animatedHint", ui->animated->isChecked());
    settings::settings().setValue("blur/qualityHint", ui->quality->isChecked());
    settings::settings().setValue("blur/performanceHint", ui->performance->isChecked());
    settings::settings().setValue("arrow/width", ui->arroww->value());
    settings::settings().setValue("arrow/height", ui->arrowh->value());
    close();
}

void BrushPenSelection::on_buttonBox_rejected() {
    close();
}

void BrushPenSelection::on_widthSlider_sliderMoved(int position) {
    ui->widthSpinner->setValue(position / 100.);
}

void BrushPenSelection::on_widthSpinner_valueChanged(double arg) {
    ui->widthSlider->setValue(arg * 100);
}

void BrushPenSelection::on_radSpinner_valueChanged(double arg1) {
    ui->radSlider->setValue(arg1 * 100);
}

void BrushPenSelection::on_radSlider_sliderMoved(int position) {
    ui->radSpinner->setValue(position / 100.);
}

void BrushPenSelection::on_alphaSpin_valueChanged(int arg1) {
    brush.setAlpha(arg1);
}

void BrushPenSelection::on_penAlphaSpin_valueChanged(int arg1) {
    pen.setAlpha(arg1);
}
