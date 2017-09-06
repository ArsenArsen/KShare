#include "monospacetextdialog.hpp"
#include "ui_monospacetextdialog.h"

MonospaceTextDialog::MonospaceTextDialog(QString name, QByteArray data, QWidget *parent)
: QDialog(parent), ui(new Ui::MonospaceTextDialog) {
    ui->setupUi(this);
    setWindowTitle(name);
    ui->textEdit->setText(data);
}

MonospaceTextDialog::~MonospaceTextDialog() {
    delete ui;
}
