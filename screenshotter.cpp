#include "screenshotter.hpp"
#include "cropeditor/cropeditor.hpp"
#include "mainwindow.hpp"
#include "screenshotutil.hpp"
#include "uploaders/uploadersingleton.hpp"
#include <QDoubleSpinBox>
#include <QTimer>
#include <settings.hpp>

void screenshotter::area() {
    CropEditor *editor = new CropEditor(screenshotutil::fullscreen(settings::settings().value("captureCursor", true).toBool()));
    QObject::connect(editor, &CropEditor::cropped, [&](QPixmap *pixmap) {
        UploaderSingleton::inst().upload(pixmap);
        QScopedPointer<CropEditor>(editor);
    });
}

void screenshotter::fullscreen() {
    UploaderSingleton::inst().upload(screenshotutil::fullscreen(settings::settings().value("captureCursor", true).toBool()));
}

void screenshotter::areaDelayed() {
    QTimer::singleShot(MainWindow::inst()->delay()->value() * 1000, &screenshotter::area);
}

void screenshotter::fullscreenDelayed() {
    QTimer::singleShot(MainWindow::inst()->delay()->value() * 1000, &screenshotter::fullscreen);
}
