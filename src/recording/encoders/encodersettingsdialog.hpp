#ifndef ENCODERSETTINGSDIALOG_HPP
#define ENCODERSETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui {
    class EncoderSettingsDialog;
}

class EncoderSettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit EncoderSettingsDialog(QWidget *parent = 0);
    ~EncoderSettingsDialog();

private slots:
    void on_defaultImageQuality_clicked(bool checked);

private:
    Ui::EncoderSettingsDialog *ui;
};

#endif // ENCODERSETTINGSDIALOG_HPP
