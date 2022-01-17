# VP8
vp8 {
INCLUDEPATH += ../libs/vpx/include
LIBS += -L../libs/vpx/$$BUILDARCH -lvpx
HEADERS += graphicsthread/yuv2rgb/bitmap.h
HEADERS += graphicsthread/yuv2rgb/yuv2rgb.h
SOURCES += graphicsthread/yuv2rgb/yuv2rgb.cpp
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_v3.cpp
HEADERS += graphicsthread/decoder_vp8.h
SOURCES += graphicsthread/decoder_vp8.cpp
HEADERS += graphicsthread/decoder_vp8_cpp.h
SOURCES += graphicsthread/decoder_vp8_cpp.cpp
}

# VP8-AVX2
vp8-avx2 {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_sse_avx2.cpp
HEADERS += graphicsthread/decoder_vp8_avx2.h
SOURCES += graphicsthread/decoder_vp8_avx2.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-avx2 {
#QMAKE_CXXFLAGS += /arch:AVX2
DEFINES += __AVX2__
}

*-g++:vp8-avx2 | *-clang:vp8-avx2 {
QMAKE_CXXFLAGS += -mavx2 -U__AVX__
}

# VP8-AVX
vp8-avx {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_avx.cpp
HEADERS += graphicsthread/decoder_vp8_avx.h
SOURCES += graphicsthread/decoder_vp8_avx.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-avx {
#QMAKE_CXXFLAGS += /arch:AVX
DEFINES += __AVX__
}

*-g++:vp8-avx | *-clang:vp8-avx {
QMAKE_CXXFLAGS += -mavx
}

# VP8-SSE
vp8-sse {
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_sse.cpp
HEADERS += graphicsthread/decoder_vp8_sse.h
SOURCES += graphicsthread/decoder_vp8_sse.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

*-msvc*:vp8-sse {
DEFINES += __SSE4_2__
}

*-g++:vp8-sse | *-clang:vp8-sse {
QMAKE_CXXFLAGS += -msse4.2
}

# VP8-NEON
vp8-neon {
# SIMD (ARM:gcc)
SOURCES += graphicsthread/yuv2rgb/yuv2rgb_neon.cpp
HEADERS += graphicsthread/decoder_vp8_neon.h
SOURCES += graphicsthread/decoder_vp8_neon.cpp
DEFINES += QTB_SIMD_SUPPORT=1
}

# multi thread converter
multi_thread_converter {
QT += concurrent
DEFINES += QTB_MULTI_THREAD_CONVERTER=1
}
else {
DEFINES += QTB_MULTI_THREAD_CONVERTER=0
}
