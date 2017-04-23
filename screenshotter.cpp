#include "screenshotter.hpp"
#include "cropeditor/cropeditor.hpp"
#include "screenshotutil.hpp"
#include "uploaders/uploadersingleton.hpp"

void screenshotter::area()
{
    CropEditor *editor = new CropEditor(screenshotutil::fullscreen());
    QObject::connect(editor, &CropEditor::cropped, [&](QPixmap *pixmap) { UploaderSingleton::inst().upload(pixmap); });
}

void screenshotter::fullscreen()
{
    UploaderSingleton::inst().upload(screenshotutil::fullscreen());
}
