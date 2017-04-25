#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <uploaders/uploader.hpp>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
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

    public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setScheme(QString scheme);

    private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *tray;

    protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_HPP
