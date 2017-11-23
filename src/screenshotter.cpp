#include "screenshotter.hpp"
#include "cropeditor/cropeditor.hpp"
#include "mainwindow.hpp"
#include "uploaders/uploadersingleton.hpp"
#include "utils.hpp"
#include <QDoubleSpinBox>
#include <QTimer>
#include <platformbackend.hpp>
#include <settings.hpp>

void screenshotter::area() {
    CropEditor *editor = new CropEditor(utils::fullscreen(settings::settings().value("captureCursor", true).toBool()));
    QObject::connect(editor, &CropEditor::cropped, [&](QPixmap pixmap) { UploaderSingleton::inst().upload(pixmap); });
}

void screenshotter::fullscreen() {
    UploaderSingleton::inst().upload(utils::fullscreen(settings::settings().value("captureCursor", true).toBool()));
}

void screenshotter::areaDelayed() {
    QTimer::singleShot(settings::settings().value("delay", 0.5).toFloat() * 1000, &screenshotter::area);
}

void screenshotter::fullscreenDelayed() {
    QTimer::singleShot(settings::settings().value("delay", 0.5).toFloat() * 1000, &screenshotter::fullscreen);
}

void screenshotter::activeDelayed() {
    QTimer::singleShot(settings::settings().value("delay", 0.5).toFloat() * 1000, &screenshotter::active);
}

void screenshotter::active() {
#ifdef PLATFORM_CAPABILITY_ACTIVEWINDOW
    UploaderSingleton::inst().upload(utils::window(PlatformBackend::inst().getActiveWID()));
#endif
}
