# ------------------------------------------------------------------------------
# Linux
# ------------------------------------------------------------------------------
linux-x64 {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_LINUX
BUILDARCH = linux-x64
}

linux-x86 {
CONFIG += desktop vp8-sse recorder
DEFINES += PLATFORM_LINUX
BUILDARCH = linux-x86
}

# Raspberry Pi 3
linux-rpi3 {
CONFIG += desktop vp8-neon
DEFINES += PLATFORM_LINUX
DEFINES += QTB_RPI3
QMAKE_CXXFLAGS += -mfpu=neon
}
