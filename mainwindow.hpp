#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSystemTrayIcon>

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

    void on_actionQuit_triggered();
    void on_actionFullscreen_triggered();
    void on_actionArea_triggered();
    void on_uploaderList_clicked(const QModelIndex &);

    public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *tray;

    protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_HPP
