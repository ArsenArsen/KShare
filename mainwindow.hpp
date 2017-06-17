#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QMap>
#include <QSystemTrayIcon>
#include <functional>

#include <recording/recordingcontroller.hpp>
#include <uploaders/uploader.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private slots:
    void quit();
    void toggleVisible();
    void newUploader(Uploader *u);

    void on_actionQuit_triggered();
    void on_actionFullscreen_triggered();
    void on_actionArea_triggered();
    void on_uploaderList_clicked(const QModelIndex &);
    void on_nameScheme_textEdited(const QString &arg1);
    void on_delay_valueChanged(double arg1);
    void on_hotkeys_doubleClicked(const QModelIndex &index);
    void on_settingsButton_clicked();
    void on_quickMode_clicked(bool checked);
    void on_hideToTray_clicked(bool checked);
    void on_actionColor_Picker_triggered();
    void on_captureCursor_clicked(bool checked);
    void on_formatBox_currentIndexChanged(int index);
    void on_imageFormatBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

    QSystemTrayIcon *tray;
    void setScheme(QString scheme);
    QDoubleSpinBox *delay();

    static MainWindow *inst();
    QMap<QString, std::function<void()>> fncs;

private:
    static MainWindow *instance;
    RecordingController *controller = new RecordingController;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_HPP
