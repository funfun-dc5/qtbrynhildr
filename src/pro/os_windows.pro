# ------------------------------------------------------------------------------
# Windows (MinGW, MSVC)
# ------------------------------------------------------------------------------
windows {
CONFIG += desktop vp8-sse vp8-avx2 recorder
DEFINES += QWT_DLL PLATFORM_WINDOWS
RC_ICONS = images/qtbrynhildr64.ico
RC_FILE = resource/qtbrynhildr.rc
#LIBS += -lwsock32 -limm32 -limagehlp -lwinmm # for WinSock
LIBS += -lws2_32 -limm32 -limagehlp -lwinmm # for WinSock2
}

msvc-windows {
CONFIG += windows
QMAKE_CXXFLAGS += /wd4819
QMAKE_LFLAGS += /LTCG
DEFINES += YY_NO_UNISTD_H
HEADERS += common/msvc.h
}

# MinGW x64
mingw-x64 {
CONFIG += windows
BUILDARCH = mingw81-x64
}

# MinGW x86
mingw-x86 {
CONFIG += windows
CONFIG -= vp8-avx2
BUILDARCH = mingw81-x86
}

# MSVC x64
msvc-x64 {
CONFIG += msvc-windows
BUILDARCH = vs2019-x64
}

# MSVC x86
msvc-x86 {
CONFIG += msvc-windows
CONFIG -= vp8-avx2
BUILDARCH = vs2019-x86
}

# MSVC x86 (Windows XP)
msvc-xp {
CONFIG += msvc-x86
CONFIG -= updatecheck help_browser
BUILDARCH = vs2015-x86
}
