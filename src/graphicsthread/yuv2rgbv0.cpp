// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "graphicsthread.h"

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

// get converter name
const char *GraphicsThread::getConverterSourceName() const
{
  return "yuv2rgbv0";
}

// YUV420 convert to RGB macro
#define GET_R(Y, V)		(Y             + 1.402 * V)
#define GET_G(Y, U, V)	(Y - 0.344 * U - 0.714 * V)
#define GET_B(Y, U)		(Y + 1.772 * U            )

// convert YUV420 to RGB24
void GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
#include "yuv2rgbv0.h"

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
#include "yuv2rgbv0.h"

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
