#include "screenshotter.hpp"
#include "cropeditor/cropeditor.hpp"
#include "mainwindow.hpp"
#include "screenshotutil.hpp"
#include "uploaders/uploadersingleton.hpp"
#include <QDoubleSpinBox>
#include <QTimer>

void screenshotter::area() {
    CropEditor *editor = new CropEditor(screenshotutil::fullscreen());
    QObject::connect(editor, &CropEditor::cropped, [&](QPixmap *pixmap) { UploaderSingleton::inst().upload(pixmap); });
}

void screenshotter::fullscreen() {
    UploaderSingleton::inst().upload(screenshotutil::fullscreen());
}

void screenshotter::areaDelayed() {
    QTimer::singleShot(MainWindow::inst()->delay()->value() * 1000, &screenshotter::area);
}

void screenshotter::fullscreenDelayed() {
    QTimer::singleShot(MainWindow::inst()->delay()->value() * 1000, &screenshotter::fullscreen);
}
