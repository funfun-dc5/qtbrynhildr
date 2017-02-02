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
TRANSLATIONS = translations/qtbrynhildr_ja_JP.ts
FORMS = GUI/connect_to_server_dialog.ui
FORMS += GUI/desktop_scaling_dialog.ui
FORMS += GUI/confirm_dialog.ui
FORMS += GUI/log_view_dialog.ui
#FORMS += GUI/software_keyboard.ui
#FORMS += GUI/software_button.ui

# for DEBUG
#CONFIG += console

# for MinGW
win32 {
DEFINES += QWT_DLL
RC_ICONS = images/qtbrynhildr64.ico
RC_FILE = resource/qtbrynhildr.rc
LIBS += -lwsock32 -lws2_32 -limm32 -limagehlp -lwinmm
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

# for Kero Keyboard
DEFINES += USE_KERO_KEYBOARD=1

# Input
HEADERS += version.h config.h parameters.h
HEADERS += common/common.h common/util.h common/protocols.h
HEADERS += machine/protocols_x86.h machine/protocols_x64.h machine/protocols_common.h

HEADERS += qtbrynhildr.h
HEADERS += option.h desktop.h
HEADERS += debug.h
HEADERS += mainwindow.h logmessage.h
HEADERS += dialog/connect_to_server_dialog.h dialog/desktop_scaling_dialog.h dialog/confirm_dialog.h dialog/log_view_dialog.h
#HEADERS += touchpanel/software_keyboard.h touchpanel/software_keyboard_jp.h touchpanel/software_keyboard_us.h
#HEADERS += touchpanel/software_button.h
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
HEADERS += windows/keycodes.h windows/eventconverter.h windows/eventconverter_jp.h windows/eventconverter_us.h windows/ntfs.h
HEADERS += function/recorder.h

SOURCES += main.cpp
SOURCES += qtbrynhildr.cpp
SOURCES += option.cpp desktop.cpp
SOURCES += debug.cpp
SOURCES += mainwindow.cpp logmessage.cpp
SOURCES += dialog/connect_to_server_dialog.cpp dialog/desktop_scaling_dialog.cpp dialog/confirm_dialog.cpp dialog/log_view_dialog.cpp
#SOURCES += touchpanel/software_keyboard.cpp touchpanel/software_keyboard_jp.cpp touchpanel/software_keyboard_us.cpp
#SOURCES += touchpanel/software_button.cpp
SOURCES += touchpanel2/software_keyboard.cpp touchpanel2/sk.cpp
SOURCES += touchpanel2/software_button.cpp touchpanel2/sb.cpp
SOURCES += settings.cpp
SOURCES += common/netthread.cpp
SOURCES += control/controlthread.cpp
SOURCES += control/keybuffer.cpp control/mousebuffer.cpp
SOURCES += graphics/graphicsthread.cpp
SOURCES += sound/soundthread.cpp
SOURCES += sound/soundbuffer.cpp
SOURCES += windows/eventconverter.cpp windows/eventconverter_jp.cpp windows/eventconverter_us.cpp windows/ntfs.cpp
SOURCES += function/recorder.cpp
