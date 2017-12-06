#ifndef BRUSHPENSELECTION_HPP
#define BRUSHPENSELECTION_HPP

#include <QDialog>
#include <cropeditor/cropscene.hpp>

namespace Ui {
    class BrushPenSelection;
}

class BrushPenSelection : public QDialog {
    Q_OBJECT

public:
    explicit BrushPenSelection(CropScene *scene);
    ~BrushPenSelection();

private slots:
    void on_alphaSpin_valueChanged(int arg1);
    void on_radSlider_sliderMoved(int position);
    void on_radSpinner_valueChanged(double arg1);
    void on_penColor_clicked(bool);
    void on_brushColor_clicked(bool);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_widthSlider_sliderMoved(int position);
    void on_widthSpinner_valueChanged(double arg1);
    void on_penAlphaSpin_valueChanged(int arg1);

private:
    Ui::BrushPenSelection *ui;
    CropScene *scene;
    QColor brush, pen;
};

#endif // BRUSHPENSELECTION_HPP
