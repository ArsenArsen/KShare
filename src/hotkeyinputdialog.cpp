#include "hotkeyinputdialog.hpp"
#include "ui_hotkeyinputdialog.h"
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>

HotkeyInputDialog::HotkeyInputDialog(QString hotkeyName, QKeySequence currentSeq, QWidget *parent)
: QDialog(parent), ui(new Ui::HotkeyInputDialog) {
    ui->setupUi(this);
    ui->keySeq->setText(currentSeq.toString());
    setWindowTitle(hotkeyName);
    connect(this, &QDialog::accepted, [&] {
        QKeySequence s(ui->keySeq->text());
        if (ui->keySeq->text().isEmpty() || !s.toString().isEmpty()) emit sequenceSelected(s, windowTitle());
    });
}

HotkeyInputDialog::~HotkeyInputDialog() {
    delete ui;
}

void HotkeyInputDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Shift || e->key() == Qt::Key_Control || e->key() == Qt::Key_Alt || e->key() == Qt::Key_AltGr
        || e->key() == Qt::Key_Context1 || e->key() == Qt::Key_Context2 || e->key() == Qt::Key_Context3
        || e->key() == Qt::Key_Context4 || e->key() == Qt::Key_Meta)
        return;
    if (recording) {
        QKeySequence seq(e->modifiers() + e->key());
        ui->keySeq->setText(seq.toString());
        recording = false;
        ui->recordButton->setText(tr("Record"));
    }
}

void HotkeyInputDialog::on_recordButton_clicked() {
    recording = !recording;
    ui->recordButton->setText(recording ? tr("Stop recording") : tr("Record"));
    if (recording)
        grabKeyboard();
    else
        releaseKeyboard();
}
