# ------------------------------------------------------------------------------
# Qt Brynhildr Ver.2
# ------------------------------------------------------------------------------
# for check spec
#message($$QMAKESPEC)

# application info.
TEMPLATE = app
TARGET = "Qt Brynhildr"
RESOURCES = qtbrynhildr.qrc
TRANSLATIONS = translations/qtbrynhildr_ja.ts

# Qt modules
QT += gui
QT += widgets
QT += multimedia

# UI
FORMS += GUI/connect_to_server_dialog.ui
FORMS += GUI/connect_to_server_dialog_touchpanel.ui
FORMS += GUI/desktop_scaling_dialog.ui
FORMS += GUI/confirm_dialog.ui
FORMS += GUI/log_view_dialog.ui

# common
INCLUDEPATH += .
# C++11
CONFIG += c++11
# Endian
DEFINES += QTB_LITTLE_ENDIAN

# for DEBUG
#CONFIG += console
#DEFINES += QTB_DEBUG

# configuration for Qt Brynhildr
#CONFIG += desktop/touchpanel celt vp8 vp8-sse vp8-avx2 vp8-neon multi_thread_converter recorder updatecheck help_browser gprof portable
CONFIG += celt vp8 multi_thread_converter updatecheck help_browser
# for XP/Vista
#CONFIG += xp

# gprof
gprof {
CONFIG += debug
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
}

# portable version
portable {
DEFINES += QTB_PORTABLE_VERSION=1
}

# ------------------------------------------------------------------------------
# Windows (MinGW, MSVC)
# ------------------------------------------------------------------------------
include (pro/os_windows.pro)

# MinGW
win32-g++ {
CONFIG += mingw-x64
# for DEBUG
CONFIG += console
}

# MSVC
win32-msvc* {
CONFIG += msvc-x64
}

# for x86
x86 {
CONFIG += msvc-x86
}

# for XP/Vista
xp {
CONFIG += msvc-xp
}

# ------------------------------------------------------------------------------
# Cygwin
# ------------------------------------------------------------------------------
cygwin-g++ {
CONFIG += cygwin-x64
#CONFIG += cygwin-x86
}
include (pro/os_cygwin.pro)

# ------------------------------------------------------------------------------
# Linux
# ------------------------------------------------------------------------------
linux-g++-64 | linux-g++ {
CONFIG += linux-x64
#CONFIG += linux-x86
}
include (pro/os_linux.pro)

# ------------------------------------------------------------------------------
# FreeBSD
# ------------------------------------------------------------------------------
freebsd-g++ | freebsd-clang {
CONFIG += freebsd-x64
}
include (pro/os_freebsd.pro)

# ------------------------------------------------------------------------------
# macOS
# ------------------------------------------------------------------------------
macx {
CONFIG += macos-x64
}
include (pro/os_macos.pro)

# ------------------------------------------------------------------------------
# iOS
# ------------------------------------------------------------------------------
include (pro/os_ios.pro)

# ------------------------------------------------------------------------------
# Android
# ------------------------------------------------------------------------------
android-g++ | android-clang {
CONFIG += android
}
include (pro/os_android.pro)

# ------------------------------------------------------------------------------
# features
# ------------------------------------------------------------------------------
# desktop/touchpanel
desktop {
CONFIG += preference
HEADERS += mainwindow/desktoppanel.h mainwindow/desktopwindow.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktopwindow.cpp
}

touchpanel {
CONFIG += software_keyboard_button
HEADERS += mainwindow/desktoppanel.h mainwindow/desktoppanelobject.h mainwindow/graphicsview.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktoppanelobject.cpp mainwindow/graphicsview.cpp
}
# config error
!desktop : !touchpanel {
message(config error : NOT found desktop or touchpanel in CONFIG)
}

# CELT
include (pro/spec_celt.pro)

# VP8
include (pro/spec_vp8.pro)

# software keyboard and button
include (pro/spec_skb.pro)

# preference
include (pro/spec_preference.pro)

# recorder
include (pro/spec_recorder.pro)

# update checker
include (pro/spec_updatechecker.pro)

# help browser
include (pro/spec_helpbrowser.pro)

# input files
HEADERS += version.h url.h config.h parameters.h
HEADERS += common/common.h common/util.h common/protocols.h
HEADERS += machine/protocols_x86.h machine/protocols_x64.h machine/protocols_common.h

HEADERS += qtbrynhildr.h
HEADERS += option.h
HEADERS += logmessage.h
HEADERS += dialog/connect_to_server_dialog.h
HEADERS += dialog/desktop_scaling_dialog.h
HEADERS += dialog/confirm_dialog.h
HEADERS += dialog/log_view_dialog.h
HEADERS += settings.h
HEADERS += util/desktop.h util/debug.h util/cpuinfo.h
HEADERS += util/ringbuffer.h
HEADERS += common/netthread.h
HEADERS += controlthread/controlthread.h
HEADERS += controlthread/keybuffer.h controlthread/mousebuffer.h
HEADERS += controlthread/mousebutton.h controlthread/mousewheel.h
HEADERS += graphicsthread/graphicsthread.h
HEADERS += graphicsthread/decoder.h
HEADERS += graphicsthread/decoder_jpeg.h
HEADERS += graphicsthread/framecounter.h
HEADERS += soundthread/soundthread.h
HEADERS += soundthread/soundbuffer.h
HEADERS += soundthread/wave.h
HEADERS += windows/eventconverter.h windows/ntfs.h windows/keycodes.h windows/keyevent.h

SOURCES += main.cpp
SOURCES += qtbrynhildr.cpp
SOURCES += option.cpp
SOURCES += util/desktop.cpp util/debug.cpp util/cpuinfo.cpp
SOURCES += util/ringbuffer.cpp
SOURCES += logmessage.cpp
SOURCES += dialog/connect_to_server_dialog.cpp
SOURCES += dialog/desktop_scaling_dialog.cpp
SOURCES += dialog/confirm_dialog.cpp
SOURCES += dialog/log_view_dialog.cpp
SOURCES += settings.cpp
SOURCES += common/netthread.cpp
SOURCES += controlthread/controlthread.cpp
SOURCES += controlthread/keybuffer.cpp controlthread/mousebuffer.cpp
SOURCES += controlthread/mousebutton.cpp controlthread/mousewheel.cpp
SOURCES += graphicsthread/graphicsthread.cpp
SOURCES += graphicsthread/decoder.cpp
SOURCES += graphicsthread/decoder_jpeg.cpp
SOURCES += graphicsthread/framecounter.cpp
SOURCES += soundthread/soundthread.cpp
SOURCES += soundthread/soundbuffer.cpp
SOURCES += windows/eventconverter.cpp windows/ntfs.cpp windows/keycodes.cpp

# for new feature
#CONFIG += new_feature
new_feature {
DEFINES += QTB_BFSH_SUPPORT=1
INCLUDEPATH += ../libs/blowfish
LIBS += -L../libs/blowfish/$$BUILDARCH -lbfsh

HEADERS += util/netutil.h
SOURCES += util/netutil.cpp
# for windows
LIBS += -liphlpapi

QMAKE_EXTRA_TARGETS += clear
clear.commands += @echo test target
}

#android-g++ | android-clang {
#QT += androidextras
#HEADERS += controlthread/android/nativebridge.h
#SOURCES += controlthread/android/nativebridge.cpp
#}
