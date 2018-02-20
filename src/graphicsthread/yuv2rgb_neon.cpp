// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif // defined(__ARM_NEON__)

// Qt Header

// Local Header
#include "graphicsthread.h"

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

#if QTB_SIMD_SUPPORT

// get converter name
const char *GraphicsThread::getConverterSourceName_SIMD() const
{
  return "yuv2rgb_neon";
}

// NEON

#if defined(__ARM_NEON__)

// convert YUV420 to RGB24
void GraphicsThread::convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_neon.h"

#if QTB_MULTI_THREAD_CONVERTER

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
void convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgb_neon.h"

#endif // QTB_MULTI_THREAD_CONVERTER

#endif // defined(__ARM_NEON__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
