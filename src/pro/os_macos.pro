# ------------------------------------------------------------------------------
# macOS
# ------------------------------------------------------------------------------
macos-x64 {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_MACOS
BUILDARCH = macos-x64
ICON = images/qtbrynhildr.icns

# for warnings
CONFIG += sdk_no_version_check
}
