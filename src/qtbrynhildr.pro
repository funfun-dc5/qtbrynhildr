# ------------------------------------------------------------------------------
# Qt Brynhildr Ver.2
# ------------------------------------------------------------------------------
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
# for check spec
# message($$QMAKESPEC)

# configuration for Qt Brynhildr
#CONFIG += desktop/touchpanel celt vp8 vp8-sse vp8-avx2 vp8-neon multi_thread_converter recorder updatecheck gprof portable
CONFIG += celt vp8 multi_thread_converter updatecheck
# for XP/Vista
#CONFIG -= updatecheck

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
win32 {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += QWT_DLL PLATFORM_WINDOWS
RC_ICONS = images/qtbrynhildr64.ico
RC_FILE = resource/qtbrynhildr.rc
#LIBS += -lwsock32 -limm32 -limagehlp -lwinmm # for WinSock
LIBS += -lws2_32 -limm32 -limagehlp -lwinmm # for WinSock2
}

# MinGW
win32-g++ {
# for DEBUG
CONFIG += console
}

# MSVC
win32-msvc* {
QMAKE_CXXFLAGS += /wd4819
QMAKE_LFLAGS += /LTCG
DEFINES += YY_NO_UNISTD_H
HEADERS += common/msvc.h
}

# ------------------------------------------------------------------------------
# Linux/Cygwin
# ------------------------------------------------------------------------------
linux-g++-64 | linux-g++ | cygwin-g++ {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_LINUX
# NEON (RaspberryPi3)
#CONFIG -= vp8-sse vp8-avx2
#CONFIG += vp8-neon
#DEFINES += QTB_RPI3
#QMAKE_CXXFLAGS += -mfpu=neon
}

# ------------------------------------------------------------------------------
# FreeBSD
# ------------------------------------------------------------------------------
freebsd-g++ | freebsd-clang {
CONFIG += desktop vp8-sse recorder
#CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_FREEBSD
}

# ------------------------------------------------------------------------------
# MacOSX
# ------------------------------------------------------------------------------
macx {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_MACOS
ICON = images/qtbrynhildr.icns
}

# ------------------------------------------------------------------------------
# iOS
# ------------------------------------------------------------------------------
ios {
CONFIG += touchpanel vp8-neon
CONFIG -= updatecheck
DEFINES += PLATFORM_MACOS QTB_IOS
BUILDARCH = ios-arm64-v8a
QMAKE_INFO_PLIST=ios/Info.plist
# icons
ios_icon.files = $$files(ios/AppIcon*.png)
QMAKE_BUNDLE_DATA =+= ios_icon
# launch images
app_launch_images.files = ios/Launch.xib $$files(ios/LaunchImage*.png)
QMAKE_BUNDLE_DATA += app_launch_images
}

# ------------------------------------------------------------------------------
# Android
# ------------------------------------------------------------------------------
android-g++ | android-clang {
TARGET = "QtBrynhildr"
CONFIG += touchpanel vp8-neon
CONFIG -= updatecheck
DEFINES += PLATFORM_LINUX QTB_ANDROID
# cpufeatures library from android-ndk
HEADERS += util/android-ndk/cpu-features.h
SOURCES += util/android-ndk/cpu-features.c

# for Android APK
# for Qt 5.14- (for Android5.0)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
BUILDARCH = android-$(OBJECTS_DIR)
ANDROID_ABIS = armeabi-v7a arm64-v8a

# dist files (java code)
DISTFILES += \
    $$ANDROID_PACKAGE_SOURCE_DIR/src/org/qtproject/mczxoxo/QtBrynhildr/QTBActivity.java

# dist files
DISTFILES += \
    $$ANDROID_PACKAGE_SOURCE_DIR/AndroidManifest.xml \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/values/libs.xml \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/values/style.xml \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-ldpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-mdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-hdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-xhdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-xxhdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/mipmap-xxxhdpi/icon.png
}

# desktop/touchpanel
desktop {
CONFIG += preference
HEADERS += mainwindow/desktoppanel.h mainwindow/desktopwindow.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktopwindow.cpp
DEFINES += QTB_TOUCHPANEL_WINDOW=0
}

touchpanel {
CONFIG += software_keyboard_button
HEADERS += mainwindow/desktoppanel.h mainwindow/desktoppanelobject.h mainwindow/graphicsview.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktoppanelobject.cpp mainwindow/graphicsview.cpp
DEFINES += QTB_TOUCHPANEL_WINDOW=1
}
# config error
!desktop : !touchpanel {
message(config error : NOT found desktop or touchpanel in CONFIG)
}

# CELT
celt {
DEFINES += QTB_CELT_SUPPORT=1
INCLUDEPATH += ../libs/celt/include
HEADERS += soundthread/converter.h soundthread/converter_celt.h
SOURCES += soundthread/converter.cpp soundthread/converter_celt.cpp
LIBS += -L../libs/celt/$$BUILDARCH -lcelt
}
else {
DEFINES += QTB_CELT_SUPPORT=0
}

# VP8
vp8 {
INCLUDEPATH += ../libs/vpx/include
LIBS += -L../libs/vpx/$$BUILDARCH -lvpx
HEADERS += graphicsthread/yuv2rgb/bitmap.h
HEADERS += graphicsthread/yuv2rgb/yuv2rgb.h
SOURCES += graphicsthread/yuv2rgb/yuv2rgb.cpp
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_v3.cpp
HEADERS += graphicsthread/decoder_vp8.h
SOURCES += graphicsthread/decoder_vp8.cpp
HEADERS += graphicsthread/decoder_vp8_cpp.h
SOURCES += graphicsthread/decoder_vp8_cpp.cpp
}

# VP8-AVX2
vp8-avx2 {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_sse_avx2.cpp
HEADERS += graphicsthread/decoder_vp8_avx2.h
SOURCES += graphicsthread/decoder_vp8_avx2.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-avx2 {
#QMAKE_CXXFLAGS += /arch:AVX2
DEFINES += __AVX2__
}

*-g++:vp8-avx2 | *-clang:vp8-avx2 {
QMAKE_CXXFLAGS += -mavx2 -U__AVX__
}

# VP8-AVX
vp8-avx {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_avx.cpp
HEADERS += graphicsthread/decoder_vp8_avx.h
SOURCES += graphicsthread/decoder_vp8_avx.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-avx {
#QMAKE_CXXFLAGS += /arch:AVX
DEFINES += __AVX__
}

*-g++:vp8-avx | *-clang:vp8-avx {
QMAKE_CXXFLAGS += -mavx
}

# VP8-SSE
vp8-sse {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_sse.cpp
HEADERS += graphicsthread/decoder_vp8_sse.h
SOURCES += graphicsthread/decoder_vp8_sse.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-sse {
DEFINES += __SSE4_2__
}

*-g++:vp8-sse | *-clang:vp8-sse {
QMAKE_CXXFLAGS += -msse4.2
}

# VP8-NEON
vp8-neon {
# SIMD (ARM:gcc)
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_neon.cpp
HEADERS += graphicsthread/decoder_vp8_neon.h
SOURCES += graphicsthread/decoder_vp8_neon.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

# multi thread converter
multi_thread_converter {
QT += concurrent
DEFINES += QTB_MULTI_THREAD_CONVERTER=1
}
else {
DEFINES += QTB_MULTI_THREAD_CONVERTER=0
}

# software keyboard and button
software_keyboard_button {
CONFIG += keylayoutfile
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=1
HEADERS += touchpanel2/software_keyboard.h touchpanel2/sk.h
SOURCES += touchpanel2/software_keyboard.cpp touchpanel2/sk.cpp
HEADERS += touchpanel2/software_button.h touchpanel2/sb.h
SOURCES += touchpanel2/software_button.cpp touchpanel2/sb.cpp
}
else {
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=0
}

# keylayout file
keylayoutfile {
HEADERS += keylayout/keylayoutfile.h
SOURCES += keylayout/keylayoutfile.cpp
HEADERS += keylayout/keylayoutfilereader.h keylayout/keylayoutfilemanager.h
SOURCES += keylayout/keylayoutfilereader.cpp keylayout/keylayoutfilemanager.cpp
HEADERS += keylayout/klfcompiler.h keylayout/klfcompiler.tab.h
SOURCES += keylayout/klfcompiler_lex.c keylayout/klfcompiler.tab.c
DEFINES += USE_KEYLAYOUTFILE YY_NO_UNPUT YY_NO_INPUT
}

# preference
preference {
DEFINES += QTB_PREFERENCE=1
FORMS += GUI/preference_dialog.ui
HEADERS += dialog/preference_dialog.h
SOURCES += dialog/preference_dialog.cpp
}
else {
DEFINES += QTB_PREFERENCE=0
}

# recorder
recorder {
DEFINES += QTB_RECORDER=1
HEADERS += function/recorder.h
SOURCES += function/recorder.cpp
}
else {
DEFINES += QTB_RECORDER=0
}

# update checker
updatecheck {
DEFINES += QTB_UPDATECHECK=1
HEADERS += util/httpgetter.h
SOURCES += util/httpgetter.cpp
}
else {
DEFINES += QTB_UPDATECHECK=0
}

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
}
