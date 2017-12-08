#include "screenoverlaysettings.hpp"
#include "ui_screenoverlaysettings.h"

#include <settings.hpp>
#include <QColorDialog>

ScreenOverlaySettings::ScreenOverlaySettings(ScreenOverlay *overlay, QWidget *parent)
: QDialog(parent), ui(new Ui::ScreenOverlaySettings), overlay(overlay) {
    ui->setupUi(this);

    ui->gridBox->setChecked(overlay->grid());
    ui->movementPattern->setCurrentIndex(overlay->movementPattern());
    highlight = overlay->highlight();
}

void ScreenOverlaySettings::on_buttonBox_accepted() {
    settings::settings().setValue("gridEnabled", ui->gridBox->isChecked());
    settings::settings().setValue("highlightColor", highlight);
    settings::settings().setValue("movementPattern", ui->movementPattern->currentIndex());
    overlay->loadSettings();
}

void ScreenOverlaySettings::on_setHighlight_pressed() {
    highlight = QColorDialog::getColor(highlight, this, tr("Highlight color"));
}

ScreenOverlaySettings::~ScreenOverlaySettings() {
    delete ui;
}
