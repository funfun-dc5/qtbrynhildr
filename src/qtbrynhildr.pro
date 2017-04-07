######################################################################
# Automatically generated by qmake (3.0) ? 3 15 00:00:00 2015
######################################################################
QT += gui
QT += widgets
QT += multimedia

TEMPLATE = app
TARGET = QtBrynhildr
INCLUDEPATH += .
RESOURCES = qtbrynhildr.qrc
TRANSLATIONS = translations/qtbrynhildr_ja.ts
FORMS = GUI/connect_to_server_dialog.ui
FORMS += GUI/desktop_scaling_dialog.ui
FORMS += GUI/confirm_dialog.ui
FORMS += GUI/log_view_dialog.ui

# C++11
CONFIG += c++11

# for DEBUG
#CONFIG += console

# for check spec
# message($$QMAKESPEC)

# for Windows (MinGW, MSVC)
win32 {
DEFINES += QWT_DLL
RC_ICONS = images/qtbrynhildr64.ico
RC_FILE = resource/qtbrynhildr.rc
LIBS += -lwsock32 -lws2_32 -limm32 -limagehlp -lwinmm
}

# for MSVC 2015
win32-msvc2015 {
CELT_SUPPORT = ON
}

# for MinGW
win32-g++ {
CELT_SUPPORT = ON
}

# for Linux/FreeBSD
linux-g++-64 | linux-g++ | freebsd-g++ {
DEFINES += QTB_RECORDER=1
CELT_SUPPORT = ON
}

# for MacOSX
macx {
DEFINES += QTB_RECORDER=1
ICON = images/qtbrynhildr.icns
CELT_SUPPORT = ON
}

# for Android
android-g++ {
DEFINES += QTB_RECORDER=0
CELT_SUPPORT = OFF
# for Android APK
DISTFILES += \
    $$PWD/../dist/android/AndroidManifest.xml \
    $$PWD/../dist/android/res/values/libs.xml \
    $$PWD/../dist/android/res/drawable-hdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-mdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-xhdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-xxhdpi/qtbrynhildr.png \
    $$PWD/../dist/android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../dist/android
}

# for crypto++
#INCLUDEPATH += ../lib/cryptopp562
#LIBS += -L../lib/cryptopp562 -lcryptopp
#HEADERS += function/cipher.h
#SOURCES += function/cipher.cpp

# for CELT
equals(CELT_SUPPORT, ON){
INCLUDEPATH += ../libs
LIBS += -L../libs -lcelt0
HEADERS += sound/converter.h sound/converter_celt.h ../libs/celt.h ../libs/celt_types.h
SOURCES += sound/converter.cpp sound/converter_celt.cpp
DEFINES += QTB_CELT_SUPPORT=1
}
else {
DEFINES += QTB_CELT_SUPPORT=0
}

# for key layout file
HEADERS += keylayout/keylayoutfile.h keylayout/keylayoutfilereader.h
SOURCES += keylayout/keylayoutfilereader.cpp

# Input
HEADERS += version.h config.h parameters.h
HEADERS += common/common.h common/util.h common/protocols.h
HEADERS += machine/protocols_x86.h machine/protocols_x64.h machine/protocols_common.h

HEADERS += qtbrynhildr.h
HEADERS += option.h desktop.h
HEADERS += debug.h
HEADERS += mainwindow.h logmessage.h
HEADERS += dialog/connect_to_server_dialog.h dialog/desktop_scaling_dialog.h dialog/confirm_dialog.h dialog/log_view_dialog.h
HEADERS += touchpanel2/software_keyboard.h touchpanel2/sk.h
HEADERS += touchpanel2/software_button.h touchpanel2/sb.h
HEADERS += settings.h
HEADERS += common/netthread.h
HEADERS += control/controlthread.h
HEADERS += control/keybuffer.h control/mousebuffer.h
HEADERS += graphics/graphicsthread.h
HEADERS += sound/soundthread.h
HEADERS += sound/soundbuffer.h
HEADERS += sound/wave.h
HEADERS += windows/eventconverter.h windows/ntfs.h windows/keycodes.h windows/keyevent.h
HEADERS += function/recorder.h

SOURCES += main.cpp
SOURCES += qtbrynhildr.cpp
SOURCES += option.cpp desktop.cpp
SOURCES += debug.cpp
SOURCES += mainwindow.cpp logmessage.cpp
SOURCES += dialog/connect_to_server_dialog.cpp dialog/desktop_scaling_dialog.cpp dialog/confirm_dialog.cpp dialog/log_view_dialog.cpp
SOURCES += touchpanel2/software_keyboard.cpp touchpanel2/sk.cpp
SOURCES += touchpanel2/software_button.cpp touchpanel2/sb.cpp
SOURCES += settings.cpp
SOURCES += common/netthread.cpp
SOURCES += control/controlthread.cpp
SOURCES += control/keybuffer.cpp control/mousebuffer.cpp
SOURCES += graphics/graphicsthread.cpp
SOURCES += sound/soundthread.cpp
SOURCES += sound/soundbuffer.cpp
SOURCES += windows/eventconverter.cpp windows/ntfs.cpp windows/keycodes.cpp
SOURCES += function/recorder.cpp
