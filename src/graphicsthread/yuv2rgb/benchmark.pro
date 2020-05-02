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
DEFINES += BENCHMARK QTB_MULTI_THREAD_CONVERTER=1 QTB_SIMD_SUPPORT=1

#CONFIG += console

# ========= benchmark spec. =========
DEFINES += LOOP_COUNT=10000 MULTI_THREAD=2
CONFIG += sse
#CONFIG += create_image
#CONFIG += load_image
# ========= benchmark spec. =========

# input files
HEADERS += yuv2rgb.h
SOURCES += benchmark.cpp yuv2rgb.cpp

cpp {
DEFINES += BENCHMARK_YUV2RGB=1
SOURCES += yuv2rgb_v3.cpp
}

sse {
DEFINES += BENCHMARK_YUV2RGB=1
SOURCES += yuv2rgb_sse.cpp
QMAKE_CXXFLAGS += -msse4.2
}

avx { # too SLOW
DEFINES += BENCHMARK_YUV2RGB=1
SOURCES += yuv2rgb_avx.cpp
QMAKE_CXXFLAGS += -mavx
}

avx2 {
DEFINES += BENCHMARK_YUV2RGB=1
SOURCES += yuv2rgb_sse_avx2.cpp
QMAKE_CXXFLAGS += -mavx2
}

neon {
DEFINES += BENCHMARK_YUV2RGB=1 __ARM_NEON__
SOURCES += yuv2rgb_neon.cpp
QMAKE_CXXFLAGS += -mfpu=neon
}

create_image {
DEFINES += BENCHMARK_CREATE_IMAGE=1
}

load_image {
DEFINES += BENCHMARK_LOAD_IMAGE=1
}
