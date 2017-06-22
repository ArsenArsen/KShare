#include "aboutbox.hpp"
#include "ui_aboutbox.h"

AboutBox::AboutBox(QWidget *parent) : QDialog(parent), ui(new Ui::AboutBox) {
    ui->setupUi(this);
    ui->label->setText(ui->label->text().arg(QApplication::applicationVersion()));
}

AboutBox::~AboutBox() {
    delete ui;
}
