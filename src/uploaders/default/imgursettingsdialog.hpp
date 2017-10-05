#ifndef IMGURSETTINGSDIALOG_HPP
#define IMGURSETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui {
    class ImgurSettingsDialog;
}

class ImgurSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImgurSettingsDialog(QWidget *parent = 0);
    ~ImgurSettingsDialog();

private slots:
    void on_addApp_clicked();
    void on_getPin_clicked();
    void on_authorize_clicked();

private:
    Ui::ImgurSettingsDialog *ui;
};

#endif // IMGURSETTINGSDIALOG_HPP
