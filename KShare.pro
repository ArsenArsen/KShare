#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T15:47:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KShare
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    cropeditor/cropeditor.cpp \
    cropeditor/cropview.cpp \
    cropeditor/cropscene.cpp \
    uploaders/uploadersingleton.cpp \
    screenshotter.cpp \
    screenshotutil.cpp \
    uploaders/default/imguruploader.cpp \
    io/ioutils.cpp \
    settings.cpp \
    uploaders/default/clipboarduploader.cpp \
    formatter.cpp \
    uploaders/customuploader.cpp \
    notifications.cpp \
    hotkeying.cpp \
    cropeditor/drawing/dotitem.cpp \
    cropeditor/settings/brushpenselection.cpp \
    cropeditor/drawing/bluritem.cpp \
    cropeditor/drawing/pathitem.cpp \
    cropeditor/drawing/lineitem.cpp \
    cropeditor/drawing/textitem.cpp \
    colorpicker/colorpickerscene.cpp \
    worker/worker.cpp \
    screenareaselector/screenareaselector.cpp \
    recording/recordingpreview.cpp \
    recording/recordingcontroller.cpp \
    recording/recordingformats.cpp \
    formats.cpp \
    recording/encoders/encoder.cpp \
    recording/encoders/encodersettings.cpp \
    recording/encoders/encodersettingsdialog.cpp \
    settingsdialog.cpp \
    aboutbox.cpp \
    cropeditor/drawing/eraseritem.cpp \
    cropeditor/drawing/rectitem.cpp \
    cropeditor/drawing/ellipseitem.cpp \
    hotkeyinputdialog.cpp \
    cropeditor/drawing/arrowitem.cpp \
    uploaders/default/imgursettingsdialog.cpp \
    uploaders/default/imgplusuploader.cpp

HEADERS  += mainwindow.hpp \
    cropeditor/cropeditor.hpp \
    cropeditor/cropview.hpp \
    cropeditor/cropscene.hpp \
    uploaders/uploader.hpp \
    uploaders/uploadersingleton.hpp \
    screenshotter.hpp \
    screenshotutil.hpp \
    uploaders/default/imguruploader.hpp \
    io/ioutils.hpp \
    settings.hpp \
    uploaders/default/clipboarduploader.hpp \
    formatter.hpp \
    uploaders/customuploader.hpp \
    notifications.hpp \
    hotkeying.hpp \
    cropeditor/drawing/drawitem.hpp \
    cropeditor/drawing/dotitem.hpp \
    cropeditor/settings/brushpenselection.hpp \
    cropeditor/drawing/bluritem.hpp \
    cropeditor/drawing/pathitem.hpp \
    cropeditor/drawing/lineitem.hpp \
    cropeditor/drawing/textitem.hpp \
    colorpicker/colorpickerscene.hpp \
    platformbackend.hpp \
    worker/worker.hpp \
    screenareaselector/screenareaselector.hpp \
    recording/recordingpreview.hpp \
    recording/recordingcontroller.hpp \
    recording/recordingformats.hpp \
    formats.hpp \
    recording/encoders/encoder.hpp \
    recording/encoders/encodersettings.hpp \
    recording/encoders/encodersettingsdialog.hpp \
    settingsdialog.hpp \
    aboutbox.hpp \
    cropeditor/drawing/eraseritem.hpp \
    cropeditor/drawing/rectitem.hpp \
    cropeditor/drawing/ellipseitem.hpp \
    hotkeyinputdialog.hpp \
    cropeditor/drawing/arrowitem.hpp \
    uploaders/default/imgursettingsdialog.hpp \
    uploaders/default/imgplusuploader.hpp

CONFIG += link_pkgconfig
PKGCONFIG += libavformat libavcodec libswscale libavutil

mac {
        SOURCES += $$PWD/platformspecifics/mac/macbackend.cpp
        HEADERS += $$PWD/platformspecifics/mac/macbackend.hpp
        LIBS += -framework Carbon
        warning(Mac is on TODO);
} else:win32 {
        SOURCES += $$PWD/platformspecifics/u32/u32backend.cpp
        HEADERS += $$PWD/platformspecifics/u32/u32backend.hpp
        LIBS += -luser32 -lkernel32 -lpthread
        QT += winextras


} else:unix {
        SOURCES += $$PWD/platformspecifics/x11/x11backend.cpp
        HEADERS += $$PWD/platformspecifics/x11/x11backend.hpp
        QT += x11extras
        LIBS += -lxcb-cursor -lxcb-xfixes -lxcb
} else {
        error(Unsupported platform);
}

FORMS    += mainwindow.ui \
    cropeditor/settings/brushpenselection.ui \
    recording/encoders/encodersettingsdialog.ui \
    settingsdialog.ui \
    aboutbox.ui \
    hotkeyinputdialog.ui \
    uploaders/default/imgursettingsdialog.ui

DISTFILES += \
    README.md \
    LICENSE \
    OlderSystemFix.patch

RESOURCES += \
    icon.qrc

ICON = icons/icon.ico

# Enable debug symbols
QMAKE_CFLAGS_DEBUG += -g

include(QHotkey/qhotkey.pri)
