# ------------------------------------------------------------------------------
# FreeBSD
# ------------------------------------------------------------------------------
freebsd-x64 {
CONFIG += desktop vp8-sse recorder
#CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_FREEBSD
BUILDARCH = freebsd-x64
}

freebsd-x86 {
CONFIG += desktop vp8-sse recorder
DEFINES += PLATFORM_FREEBSD
BUILDARCH = freebsd-x86
}
