# ------------------------------------------------------------------------------
# iOS
# ------------------------------------------------------------------------------
ios {
CONFIG += touchpanel vp8-neon
CONFIG -= updatecheck help_browser
DEFINES += PLATFORM_MACOS
BUILDARCH = ios-arm64-v8a
QMAKE_INFO_PLIST=ios/Info.plist
# icons
ios_icon.files = $$files(ios/AppIcon*.png)
QMAKE_BUNDLE_DATA += ios_icon
# launch images
#app_launch_images.files = ios/Launch.xib $$files(ios/LaunchImage*.png)
#QMAKE_BUNDLE_DATA += app_launch_images

QMAKE_ASSET_CATALOGS = ios/Images.xcassets
QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"
}
