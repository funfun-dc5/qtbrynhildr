// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "graphicsthread.h"

// for TEST
#define PRINT_CALC_RATE 0

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

// get converter name
const char *GraphicsThread::getConverterSourceName() const
{
  return "yuv2rgbv3"; // 136 fps
}

// convert YUV420 to RGB24
void GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgbv3.h"

#if QTB_MULTI_THREAD_CONVERTER

// for qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
// clip
inline int clip(int val)
{
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
void convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgbv3.h"

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
