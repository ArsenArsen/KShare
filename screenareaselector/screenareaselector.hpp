#ifndef SCREENAREASELECTOR_HPP
#define SCREENAREASELECTOR_HPP

#include <QWidget>

class ScreenAreaSelector : public QWidget {
    Q_OBJECT
public:
    ScreenAreaSelector();
    ~ScreenAreaSelector();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *) override;
signals:
    void selectedArea(QRect area);
};

#endif // SCREENAREASELECTOR_HPP
