// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if _MSC_VER
#include <intrin.h>
#else // _MSC_VER
#include <x86intrin.h>
#endif // _MSC_VER

// Qt Header

// Local Header
#include "graphicsthread.h"

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

#if QTB_SIMD_SUPPORT

// get converter name
const char *GraphicsThread::getConverterSourceName_SIMD() const
{
#if defined(__AVX2__)
  return "yuv2rgb_sse:avx2"; // 172 fps
#else // defined(__AVX2__)
  return "yuv2rgb_sse";		 // 159 fps
#endif // defined(__AVX2__)
}

// SSE

#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)

// GraphicsThread::convert YUV420 to RGB24
void GraphicsThread::convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_sse.h"

#if QTB_MULTI_THREAD_CONVERTER

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
void convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_sse.h"

#endif // QTB_MULTI_THREAD_CONVERTER

#endif // defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
