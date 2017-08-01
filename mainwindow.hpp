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

    void on_actionQuit_triggered();
    void on_actionFullscreen_triggered();
    void on_actionArea_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionSettings_triggered();
    void on_actionColor_Picker_triggered();
    void on_actionAbout_triggered();
    void on_actionActive_window_triggered();
    void on_actionAbort_triggered();

public:
    static MainWindow *inst();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool valid();
    Ui::MainWindow *ui;
    RecordingController *controller = new RecordingController;

    QSystemTrayIcon *tray;
public slots:
    void rec();

private:
    bool val = false;
    QMenu *menu;
    static MainWindow *instance;

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_HPP
