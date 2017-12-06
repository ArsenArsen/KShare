#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T15:47:09
#
#-------------------------------------------------

QT       += core gui network widgets svg

TARGET = kshare
TEMPLATE = app

CONFIG += c++11 thread

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
    utils.cpp \
    uploaders/default/imguruploader.cpp \
    io/ioutils.cpp \
    settings.cpp \
    uploaders/default/clipboarduploader.cpp \
    formatter.cpp \
    uploaders/customuploader.cpp \
    notifications.cpp \
    hotkeying.cpp \
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
    uploaders/default/imgplusuploader.cpp \
    filenamevalidator.cpp \
    logs/requestlogging.cpp \
    logs/historydialog.cpp \
    monospacetextdialog.cpp \
    screenoverlayview.cpp \
    cropeditor/selectionrectangle.cpp \
    screenoverlay.cpp

HEADERS  += mainwindow.hpp \
    cropeditor/cropeditor.hpp \
    cropeditor/cropview.hpp \
    cropeditor/cropscene.hpp \
    uploaders/uploader.hpp \
    uploaders/uploadersingleton.hpp \
    screenshotter.hpp \
    utils.hpp \
    uploaders/default/imguruploader.hpp \
    io/ioutils.hpp \
    settings.hpp \
    uploaders/default/clipboarduploader.hpp \
    formatter.hpp \
    uploaders/customuploader.hpp \
    notifications.hpp \
    hotkeying.hpp \
    cropeditor/drawing/drawitem.hpp \
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
    uploaders/default/imgplusuploader.hpp \
    filenamevalidator.hpp \
    logs/requestlogging.hpp \
    logs/historydialog.hpp \
    screenoverlayview.hpp \
    screenoverlayview.hpp \
    monospacetextdialog.hpp \
    cropeditor/selectionrectangle.hpp \
    screenoverlay.hpp

nopkg {
#    win32 {
#        !exists($$(FFMPEG_DEV_PATH)\README.txt) {
#                error("You must define a valid FFMPEG_DEV_PATH")
#        }

#        !exists($$(FFMPEG_SHARED_PATH)\README.txt) {
#                error("You must define a valid FFMPEG_SHARED_PATH")
#        }

#        INCLUDEPATH += $$(FFMPEG_DEV_PATH)\include
#        LIBS += -L$$(FFMPEG_DEV_PATH)\lib
#        message(Set the library and include paths);
#    }
    LIBS += -lavcodec -lavformat -lavutil -lswscale
    message(nopkg);
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += libavformat libavcodec libswscale libavutil
}

mac {
    ICON = $$PWD/icons/icon.icns
    SOURCES += $$PWD/platformspecifics/mac/macbackend.cpp
    HEADERS += $$PWD/platformspecifics/mac/macbackend.hpp
    LIBS += -framework Carbon
    QMAKE_INFO_PLIST = $$PWD/../packages/macos/Info.plist
    warning(Mac is on TODO);
} else:win32 {
    RC_FILE = $$PWD/icon.rc
    SOURCES += $$PWD/platformspecifics/u32/u32backend.cpp
    HEADERS += $$PWD/platformspecifics/u32/u32backend.hpp
    LIBS += -luser32 -lkernel32 -lpthread
    QT += winextras
} else:unix {
    RC_FILE = $$PWD/icon.rc
    SOURCES += $$PWD/platformspecifics/x11/x11backend.cpp
    HEADERS += $$PWD/platformspecifics/x11/x11backend.hpp
    QT += x11extras
    LIBS += -lxcb-cursor -lxcb-xfixes -lxcb

    target.path = bin/
    icon.files = $$PWD/icons/KShare.png
    icon.path = share/pixmaps/
    desktop.files = $$PWD/../KShare.desktop
    desktop.path = share/applications/
    INSTALLS += target icon desktop
} else {
    error(Unsupported platform);
}

FORMS    += mainwindow.ui \
    cropeditor/settings/brushpenselection.ui \
    recording/encoders/encodersettingsdialog.ui \
    settingsdialog.ui \
    aboutbox.ui \
    hotkeyinputdialog.ui \
    uploaders/default/imgursettingsdialog.ui \
    logs/historydialog.ui \
    monospacetextdialog.ui

RESOURCES += \
    icon.qrc \
    translations.qrc

QMAKE_CFLAGS_DEBUG += -g

include($$PWD/../QHotkey/qhotkey.pri)
