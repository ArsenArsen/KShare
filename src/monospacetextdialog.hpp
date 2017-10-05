#ifndef MONOSPACETEXTDIALOG_H
#define MONOSPACETEXTDIALOG_H

#include <QDialog>

namespace Ui {
    class MonospaceTextDialog;
}

class MonospaceTextDialog : public QDialog {
    Q_OBJECT

public:
    explicit MonospaceTextDialog(QString name, QByteArray data, QWidget *parent = 0);
    ~MonospaceTextDialog();

private:
    Ui::MonospaceTextDialog *ui;
};

#endif // MONOSPACETEXTDIALOG_H
