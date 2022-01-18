# ------------------------------------------------------------------------------
# Cygwin
# ------------------------------------------------------------------------------
# Cygwin x64
cygwin-x64 {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += PLATFORM_LINUX
BUILDARCH = cygwin-x64
}

# Cygwin x86
cygwin-x86 {
CONFIG += desktop vp8-sse recorder
DEFINES += PLATFORM_LINUX
BUILDARCH = cygwin-x86
}
