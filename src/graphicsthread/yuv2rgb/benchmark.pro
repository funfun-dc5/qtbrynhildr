# ------------------------------------------------------------------------------
# benchmark
# ------------------------------------------------------------------------------
# application info.
TEMPLATE = app
TARGET = benchmark
INCLUDEPATH += . ../..

# Qt modules
QT += widgets
QT += concurrent

# spec.
DEFINES += BENCHMARK QTB_MULTI_THREAD_CONVERTER

# ========= benchmark spec. =========
DEFINES += LOOP_COUNT=10000 MULTI_THREAD=2
CONFIG += sse
# ========= benchmark spec. =========

# input files
HEADERS += yuv2rgb.h
SOURCES += benchmark.cpp yuv2rgb.cpp

cpp {
SOURCES += yuv2rgb_v3.cpp
}

sse {
SOURCES += yuv2rgb_sse.cpp
QMAKE_CXXFLAGS += -msse4.2
}

avx { # too SLOW
SOURCES += yuv2rgb_avx.cpp
QMAKE_CXXFLAGS += -mavx
}

avx2 {
SOURCES += yuv2rgb_sse_avx2.cpp
QMAKE_CXXFLAGS += -mavx2
}
