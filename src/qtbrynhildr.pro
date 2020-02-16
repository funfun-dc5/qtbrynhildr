######################################################################
# Automatically generated by qmake (3.0) ? 3 15 00:00:00 2015
######################################################################
QT += gui
QT += widgets
QT += multimedia

TEMPLATE = app
TARGET = "Qt Brynhildr"
INCLUDEPATH += .
RESOURCES = qtbrynhildr.qrc
TRANSLATIONS = translations/qtbrynhildr_ja.ts
FORMS += GUI/connect_to_server_dialog.ui
FORMS += GUI/connect_to_server_dialog_touchpanel.ui
FORMS += GUI/desktop_scaling_dialog.ui
FORMS += GUI/confirm_dialog.ui
FORMS += GUI/log_view_dialog.ui

# C++11
CONFIG += c++11

# for DEBUG
#CONFIG += console

# for check spec
# message($$QMAKESPEC)

# Endian
DEFINES += QTB_LITTLE_ENDIAN

# for gprof
#CONFIG += debug
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# for portable version
#DEFINES += QTB_PORTABLE_VERSION=1

# for multi thread converter
QT += concurrent
DEFINES += QTB_MULTI_THREAD_CONVERTER=1

# for Windows (MinGW, MSVC)
win32 {
DEFINES += QWT_DLL PLATFORM_WINDOWS
RC_ICONS = images/qtbrynhildr64.ico
RC_FILE = resource/qtbrynhildr.rc
DESKTOP = ON
CELT_SUPPORT = ON
INCLUDEPATH += ../libs/vpx
#LIBS += -lwsock32 -limm32 -limagehlp -lwinmm # for WinSock
LIBS += -lws2_32 -limm32 -limagehlp -lwinmm # for WinSock2
LIBS += -L../libs/vpx -lvpx
LIBS += -L../libs/celt -lcelt
}

# for MSVC
win32-msvc* {
#CELT_SUPPORT = ON
QMAKE_CXXFLAGS += /wd4819
QMAKE_LFLAGS += /LTCG
DEFINES += YY_NO_UNISTD_H
HEADERS += common/msvc.h
# SIMD (INTEL:MSVC)
HEADERS += yuv2rgb/yuv2rgb.h
SOURCES += yuv2rgb/yuv2rgb_v3.cpp
# SSE4.1
SOURCES += yuv2rgb/yuv2rgb_sse.cpp
DEFINES += __SSE4_1__
# AVX
#SOURCES += yuv2rgb/yuv2rgb_avx.cpp
#QMAKE_CXXFLAGS += /arch:AVX
# AVX2
#SOURCES += yuv2rgb/yuv2rgb_sse_avx2.cpp
#QMAKE_CXXFLAGS += /arch:AVX2
}

# for MinGW
win32-g++ {
CONFIG += console
#LIBS += -static-libgcc -static-libstdc++
#CELT_SUPPORT = ON
# SIMD (INTEL:gcc/clang)
HEADERS += yuv2rgb/yuv2rgb.h
SOURCES += yuv2rgb/yuv2rgb_v3.cpp
# SSE4.1
SOURCES += yuv2rgb/yuv2rgb_sse.cpp
QMAKE_CXXFLAGS += -msse4.1
# AVX
#SOURCES += yuv2rgb/yuv2rgb_avx.cpp
#QMAKE_CXXFLAGS += -mavx
# AVX2
#SOURCES += yuv2rgb/yuv2rgb_sse_avx2.cpp
#QMAKE_CXXFLAGS += -mavx2
}

# for Linux/FreeBSD
linux-g++-64 | linux-g++ | freebsd-g++ {
DEFINES += QTB_RECORDER=1 PLATFORM_LINUX
DESKTOP = ON
CELT_SUPPORT = ON
INCLUDEPATH += ../libs/vpx
LIBS += -L../libs/vpx -lvpx
#INCLUDEPATH += /usr/include/vpx
#LIBS += -lvpx
LIBS += -L../libs/celt -lcelt
# SIMD (INTEL:gcc/clang)
HEADERS += yuv2rgb/yuv2rgb.h
SOURCES += yuv2rgb/yuv2rgb_v3.cpp
# SSE4.1
SOURCES += yuv2rgb/yuv2rgb_sse.cpp
QMAKE_CXXFLAGS += -msse4.1
# AVX
#SOURCES += yuv2rgb/yuv2rgb_avx.cpp
#QMAKE_CXXFLAGS += -mavx
# AVX2
#SOURCES += yuv2rgb/yuv2rgb_sse_avx2.cpp
#QMAKE_CXXFLAGS += -mavx2
# NEON (RaspberryPi3)
#DEFINES += QTB_RPI3
#SOURCES += yuv2rgb/yuv2rgb_neon.cpp
#QMAKE_CXXFLAGS += -mfpu=neon
}

# for MacOSX
macx {
DEFINES += QTB_RECORDER=1 PLATFORM_MACOS
ICON = images/qtbrynhildr.icns
DESKTOP = ON
CELT_SUPPORT = ON
INCLUDEPATH += ../libs/vpx
LIBS += -L../libs/vpx -lvpx
LIBS += -L../libs/celt -lcelt
# SIMD (INTEL:gcc/clang)
HEADERS += yuv2rgb/yuv2rgb.h
SOURCES += yuv2rgb/yuv2rgb_v3.cpp
# SSE4.1
SOURCES += yuv2rgb/yuv2rgb_sse.cpp
QMAKE_CXXFLAGS += -msse4.1
# AVX
#SOURCES += yuv2rgb/yuv2rgb_avx.cpp
#QMAKE_CXXFLAGS += -mavx
}

# for Android
android-g++ | android-clang {
DEFINES += QTB_RECORDER=0 PLATFORM_LINUX
DESKTOP = OFF
CELT_SUPPORT = ON
INCLUDEPATH += ../libs/vpx
LIBS += -L../libs/vpx -lvpx_android_armv7
LIBS += -L../libs/celt -lcelt_android_armv7
# cpufeatures library from android-ndk
HEADERS += util/android-ndk/cpu-features.h
SOURCES += util/android-ndk/cpu-features.c
# SIMD (ARM:gcc)
HEADERS += yuv2rgb/yuv2rgb.h
SOURCES += yuv2rgb/yuv2rgb_v3.cpp
SOURCES += yuv2rgb/yuv2rgb_neon.cpp
QMAKE_CXXFLAGS += -mfpu=neon
# for Android APK
DISTFILES += \
    $$PWD/../dist/android/AndroidManifest.xml \
    $$PWD/../dist/android/res/values/libs.xml \
    $$PWD/../dist/android/res/values/style.xml \
    $$PWD/../dist/android/res/drawable-hdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-mdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-xhdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/drawable-xxhdpi/qtbrynhildr.png \
    $$PWD/../dist/android/res/mipmap/qtbrynhildr.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../dist/android
}

equals(DESKTOP, ON){
SOFTWARE_KEYBOARD_BUTTON_SUPPORT = OFF
PREFERENCE_DIALOG = ON
HEADERS += mainwindow/desktoppanel.h mainwindow/desktopwindow.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktopwindow.cpp
DEFINES += QTB_NEW_DESKTOPWINDOW=0
}
else {
SOFTWARE_KEYBOARD_BUTTON_SUPPORT = ON
PREFERENCE_DIALOG = OFF
HEADERS += mainwindow/desktoppanel.h mainwindow/desktoppanelobject.h mainwindow/graphicsview.h
SOURCES += mainwindow/desktoppanel.cpp mainwindow/desktoppanelobject.cpp mainwindow/graphicsview.cpp
DEFINES += QTB_NEW_DESKTOPWINDOW=1
}

# for crypto++
#INCLUDEPATH += ../lib/cryptopp562
#LIBS += -L../lib/cryptopp562 -lcryptopp
#HEADERS += function/cipher.h
#SOURCES += function/cipher.cpp

# for Software Keyboard/Button
equals(SOFTWARE_KEYBOARD_BUTTON_SUPPORT, ON){
HEADERS += touchpanel2/software_keyboard.h touchpanel2/sk.h
HEADERS += touchpanel2/software_button.h touchpanel2/sb.h
SOURCES += touchpanel2/software_keyboard.cpp touchpanel2/sk.cpp
SOURCES += touchpanel2/software_button.cpp touchpanel2/sb.cpp
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=1
}
else {
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=0
}

# for CELT
equals(CELT_SUPPORT, ON){
INCLUDEPATH += ../libs/celt
HEADERS += soundthread/converter.h soundthread/converter_celt.h
SOURCES += soundthread/converter.cpp soundthread/converter_celt.cpp
#INCLUDEPATH += ../libs/opus
#LIBS += -L../libs/opus -lopus
#HEADERS += soundthread/converter.h soundthread/converter_opus.h
#SOURCES += soundthread/converter.cpp soundthread/converter_opus.cpp
DEFINES += QTB_CELT_SUPPORT=1
}
else {
DEFINES += QTB_CELT_SUPPORT=0
}

# for key layout file
KEYLAYOUTFILE_SUPPORT=ON
equals(KEYLAYOUTFILE_SUPPORT, ON){
HEADERS += keylayout/keylayoutfile.h keylayout/keylayoutfilereader.h keylayout/keylayoutfilemanager.h
SOURCES += keylayout/keylayoutfile.cpp keylayout/keylayoutfilereader.cpp keylayout/keylayoutfilemanager.cpp
HEADERS += keylayout/klfcompiler.h keylayout/klfcompiler.tab.h
SOURCES += keylayout/klfcompiler_lex.c keylayout/klfcompiler.tab.c
DEFINES += USE_KEYLAYOUTFILE YY_NO_UNPUT YY_NO_INPUT
}

# for update check
HEADERS += util/httpgetter.h
SOURCES += util/httpgetter.cpp

# for preference dialog
equals(PREFERENCE_DIALOG, ON){
FORMS += GUI/preference_dialog.ui
HEADERS += dialog/preference_dialog.h
SOURCES += dialog/preference_dialog.cpp
DEFINES += QTB_PREFERENCE=1
}
else {
DEFINES += QTB_PREFERENCE=0
}

# Input
HEADERS += version.h config.h parameters.h
HEADERS += common/common.h common/util.h common/protocols.h
HEADERS += machine/protocols_x86.h machine/protocols_x64.h machine/protocols_common.h

HEADERS += qtbrynhildr.h
HEADERS += option.h
HEADERS += logmessage.h
HEADERS += dialog/connect_to_server_dialog.h dialog/desktop_scaling_dialog.h dialog/confirm_dialog.h dialog/log_view_dialog.h
HEADERS += settings.h
HEADERS += util/desktop.h util/debug.h util/cpuinfo.h
HEADERS += util/ringbuffer.h
HEADERS += common/netthread.h
HEADERS += controlthread/controlthread.h
HEADERS += controlthread/keybuffer.h controlthread/mousebuffer.h
HEADERS += controlthread/mousebutton.h controlthread/mousewheel.h
HEADERS += graphicsthread/graphicsthread.h
HEADERS += soundthread/soundthread.h
HEADERS += graphicsthread/graphicsbuffer.h
HEADERS += soundthread/soundbuffer.h
HEADERS += soundthread/wave.h
HEADERS += windows/eventconverter.h windows/ntfs.h windows/keycodes.h windows/keyevent.h
HEADERS += function/recorder.h
HEADERS += yuv2rgb/yuv2rgb.h

SOURCES += main.cpp
SOURCES += qtbrynhildr.cpp
SOURCES += option.cpp
SOURCES += util/desktop.cpp util/debug.cpp util/cpuinfo.cpp
SOURCES += util/ringbuffer.cpp
SOURCES += logmessage.cpp
SOURCES += dialog/connect_to_server_dialog.cpp dialog/desktop_scaling_dialog.cpp dialog/confirm_dialog.cpp dialog/log_view_dialog.cpp
SOURCES += settings.cpp
SOURCES += common/netthread.cpp
SOURCES += controlthread/controlthread.cpp
SOURCES += controlthread/keybuffer.cpp controlthread/mousebuffer.cpp
SOURCES += controlthread/mousebutton.cpp controlthread/mousewheel.cpp
SOURCES += graphicsthread/graphicsthread.cpp
SOURCES += soundthread/soundthread.cpp
SOURCES += graphicsthread/graphicsbuffer.cpp
SOURCES += soundthread/soundbuffer.cpp
SOURCES += windows/eventconverter.cpp windows/ntfs.cpp windows/keycodes.cpp
SOURCES += function/recorder.cpp
SOURCES += yuv2rgb/yuv2rgb.cpp

# for TEST (QTB_TEST_xxxx)
DEFINES += QTB_TEST_TOUCHPANEL_ON_DESKTOP=0

# for new feature
NEW_FEATURE = ON
equals(NEW_FEATURE, ON){
HEADERS += yuv2rgb/bitmap.h

HEADERS += graphicsthread/decoder.h
HEADERS += graphicsthread/decoder_jpeg.h
HEADERS += graphicsthread/decoder_vp8.h
HEADERS += graphicsthread/decoder_vp8_cpp.h

SOURCES += graphicsthread/decoder.cpp
SOURCES += graphicsthread/decoder_jpeg.cpp
SOURCES += graphicsthread/decoder_vp8.cpp
SOURCES += graphicsthread/decoder_vp8_cpp.cpp

# for INTEL cpu
HEADERS += graphicsthread/decoder_vp8_sse.h
HEADERS += graphicsthread/decoder_vp8_avx2.h
SOURCES += graphicsthread/decoder_vp8_sse.cpp
SOURCES += graphicsthread/decoder_vp8_avx2.cpp

SOURCES += yuv2rgb/yuv2rgb_sse_avx2.cpp
QMAKE_CXXFLAGS += -mavx2
#QMAKE_CXXFLAGS += /arch:AVX2

# for ARM cpu
#HEADERS += graphicsthread/decoder_vp8_neon.h
#SOURCES += graphicsthread/decoder_vp8_neon.cpp

HEADERS += graphicsthread/framecounter.h
SOURCES += graphicsthread/framecounter.cpp

HEADERS += graphicsthread/framecontroler.h
SOURCES += graphicsthread/framecontroler.cpp
}
