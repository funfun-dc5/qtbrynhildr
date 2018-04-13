// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef YUV2RGB_H
#define YUV2RGB_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


// RGB image format
#if USE_PPM_LOADER_FOR_VP8
#define FORMAT_RGB888 1 // fixed
#else // USE_PPM_LOADER_FOR_VP8
//#define FORMAT_RGB888 1
#define FORMAT_RGBA8888 1
#endif // USE_PPM_LOADER_FOR_VP8

// RGB888 (3bytes) format info.
#if FORMAT_RGB888
#define IMAGE_FORMAT QImage::Format_RGB888
#define IMAGE_FORMAT_SIZE 3
#endif // FORMAT_RGB888

// RGBA8888 (4bytes) format info.
#if FORMAT_RGBA8888
#define IMAGE_FORMAT QImage::Format_RGBA8888
#define IMAGE_FORMAT_SIZE 4
#endif // FORMAT_RGBA8888

namespace qtbrynhildr {

#if QTB_MULTI_THREAD_CONVERTER

// parameters
extern int width;
extern int uvNext;
extern int rgb24Next;

extern uchar *yuv420;
extern uchar *yuv1;
extern uchar *y1topOrg;
extern uchar *u1topOrg;
extern uchar *v1topOrg;
extern uchar *y2topOrg;
extern uchar *u2topOrg;
extern uchar *v2topOrg;

// YUV420 convert to RGB24
extern void convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);

#if QTB_SIMD_SUPPORT
// YUV420 convert to RGB24 (SIMD version)
extern void convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);
#endif // QTB_SIMD_SUPPORT

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // YUV2RGB_H
