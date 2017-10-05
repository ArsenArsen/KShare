#ifndef HOTKEYINPUTDIALOG_HPP
#define HOTKEYINPUTDIALOG_HPP

#include <QDialog>

namespace Ui {
    class HotkeyInputDialog;
}

class HotkeyInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit HotkeyInputDialog(QString hotkeyName, QKeySequence currentSeq, QWidget *parent = 0);
    ~HotkeyInputDialog();

signals:
    void sequenceSelected(QKeySequence seq, QString name);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void on_recordButton_clicked();

private:
    bool recording = false;
    Ui::HotkeyInputDialog *ui;
};

#endif // HOTKEYINPUTDIALOG_HPP
