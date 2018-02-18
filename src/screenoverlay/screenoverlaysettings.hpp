#ifndef SCREENOVERLAYSETTINGS_H
#define SCREENOVERLAYSETTINGS_H

#include "screenoverlay.hpp"
#include <QDialog>

namespace Ui {
    class ScreenOverlaySettings;
}

class ScreenOverlaySettings : public QDialog {
    Q_OBJECT

public:
    explicit ScreenOverlaySettings(ScreenOverlay *overlay, QWidget *parent = 0);
    ~ScreenOverlaySettings();

private slots:
    void on_buttonBox_accepted();
    void on_setHighlight_pressed();

private:
    Ui::ScreenOverlaySettings *ui;
    QColor highlight;
    ScreenOverlay *overlay;
};

#endif // SCREENOVERLAYSETTINGS_H
