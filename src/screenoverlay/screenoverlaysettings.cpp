#include "screenoverlaysettings.hpp"
#include "ui_screenoverlaysettings.h"

#include <QColorDialog>
#include <settings.hpp>

ScreenOverlaySettings::ScreenOverlaySettings(ScreenOverlay *overlay, QWidget *parent)
: QDialog(parent), ui(new Ui::ScreenOverlaySettings), overlay(overlay) {
    ui->setupUi(this);

    ui->gridBox->setChecked(overlay->grid());
    ui->movementPattern->setCurrentIndex(overlay->movementPattern());
    highlight = overlay->highlight();
    fg = overlay->foreground();
    ui->preview->setStyleSheet(QString("background-color: %1; color: %2;").arg(highlight.name()).arg(fg.name()));
}

void ScreenOverlaySettings::on_buttonBox_accepted() {
    settings::settings().setValue("gridEnabled", ui->gridBox->isChecked());
    settings::settings().setValue("highlightColor", highlight);
    settings::settings().setValue("foregroundColor", fg);
    settings::settings().setValue("movementPattern", ui->movementPattern->currentIndex());
    overlay->loadSettings();
}

void ScreenOverlaySettings::on_fgColor_pressed() {
    QColor fog = QColorDialog::getColor("Foreground color", this, tr("Foreground color"));
    if (!fog.isValid()) return;
    fg = fog;
    ui->preview->setStyleSheet(QString("background-color: %1; color: %2;").arg(highlight.name()).arg(fg.name()));
}

void ScreenOverlaySettings::on_setHighlight_pressed() {
    QColor hl = QColorDialog::getColor(highlight, this, tr("Highlight color"));
    if (!hl.isValid()) return;
    highlight = hl;
    ui->preview->setStyleSheet(QString("background-color: %1; color: %2;").arg(highlight.name()).arg(fg.name()));
}

ScreenOverlaySettings::~ScreenOverlaySettings() {
    delete ui;
}
