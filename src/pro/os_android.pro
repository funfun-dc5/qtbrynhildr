# ------------------------------------------------------------------------------
# Android
# ------------------------------------------------------------------------------
android {
TARGET = "QtBrynhildr"
CONFIG += touchpanel vp8-neon
CONFIG -= help_browser
DEFINES += PLATFORM_LINUX QTB_ANDROID
# cpufeatures library from android-ndk
HEADERS += util/android-ndk/cpu-features.h
SOURCES += util/android-ndk/cpu-features.c

# for Android APK
# for Qt 5.14- (for Android5.0)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android-qt5
BUILDARCH = android-$(OBJECTS_DIR)
ANDROID_ABIS = armeabi-v7a arm64-v8a
# for Qt 6.4.2 32bit
#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android-qt6
#BUILDARCH = android-armeabi-v7a-android23
# for Qt 6.4.2 64bit
#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android-qt6
#BUILDARCH = android-arm64-v8a-android31


# for OpenSSL
include(C:/Tools/develop/android-sdk-windows/android_openssl/openssl.pri)

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
