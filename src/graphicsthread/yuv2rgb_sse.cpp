// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(_MSC_VER)
#include <intrin.h>
#else // defined(_MSC_VER)
#include <x86intrin.h>
#endif // defined(_MSC_VER)

// Qt Header

// Local Header
#include "graphicsthread.h"

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

// get converter name
const char *GraphicsThread::getConverterSourceName() const
{
  return "yuv2rgb_sse";
}

// SSE

#if defined(__SSE4_1__) || defined(__AVX__)

// GraphicsThread::convert YUV420 to RGB24
void GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_sse.h"

#if QTB_MULTI_THREAD_CONVERTER

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
void convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_sse.h"

#endif // QTB_MULTI_THREAD_CONVERTER

#endif // defined(__SSE4_1__) || defined(__AVX__)

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
