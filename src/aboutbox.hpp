#ifndef ABOUTBOX_HPP
#define ABOUTBOX_HPP

#include <QDialog>

namespace Ui {
    class AboutBox;
}

class AboutBox : public QDialog {
    Q_OBJECT

public:
    explicit AboutBox(QWidget *parent = 0);
    ~AboutBox();

private:
    Ui::AboutBox *ui;
};

#endif // ABOUTBOX_HPP
