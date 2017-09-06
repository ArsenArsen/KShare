#ifndef SCREENAREASELECTOR_HPP
#define SCREENAREASELECTOR_HPP

#include <QLabel>
#include <QWidget>

class ScreenAreaSelector : public QWidget {
    Q_OBJECT
public:
    ScreenAreaSelector();
    ~ScreenAreaSelector();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *) override;
    void resizeEvent(QResizeEvent *) override;
signals:
    void selectedArea(QRect area);

private:
    QLabel *hintLabel;
};

#endif // SCREENAREASELECTOR_HPP
