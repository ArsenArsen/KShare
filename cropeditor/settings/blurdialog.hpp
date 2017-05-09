#ifndef BLURDIALOG_HPP
#define BLURDIALOG_HPP

#include <QDialog>
#include <QGraphicsBlurEffect>

namespace Ui {
class BlurDialog;
}

class BlurDialog : public QDialog {
    Q_OBJECT

    public:
    explicit BlurDialog(QGraphicsBlurEffect *effect, QWidget *parent = 0);
    ~BlurDialog();

    private slots:
    void on_radSpinner_valueChanged(double arg1);
    void on_radSlider_sliderMoved(int position);

    private:
    Ui::BlurDialog *ui;
    QGraphicsBlurEffect *effect;
};

#endif // BLURDIALOG_HPP
