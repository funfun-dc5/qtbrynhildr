// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef YUV2RGB_H
#define YUV2RGB_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


// libvxp Header
#include "vpx_decoder.h"
#include "vp8dx.h"

// desktop image format
#define FORMAT_RGB32 1
//#define FORMAT_RGB888 1

#if FORMAT_RGB32
// RGB32 (4bytes) format info.
#define IMAGE_FORMAT QImage::Format_RGB32
#define IMAGE_FORMAT_SIZE 4
#elif FORMAT_RGB888
// RGB888 (3bytes) format info.
#define IMAGE_FORMAT QImage::Format_RGB888
#define IMAGE_FORMAT_SIZE 3
#else  // FORMAT_RGB888
#error "unknown image format!"
#endif

namespace qtbrynhildr {

#if QTB_MULTI_THREAD_CONVERTER

// parameters
extern int width;
extern int height;
extern int imageSize;
extern int uvNext;
extern int rgbNext;

extern uchar *yuv;
extern uchar *yuv1;
extern uchar *yuv2;
extern uchar *y1topOrg;
extern uchar *u1topOrg;
extern uchar *v1topOrg;
extern uchar *y2topOrg;
extern uchar *u2topOrg;
extern uchar *v2topOrg;

extern uchar *rgb;

// codec context
extern vpx_codec_ctx_t c_codec;

// initialize for yuv
extern void initVPX();

// decode VP8
extern void decodeVPX(uchar *buffer, int size);

// setup for yuv, rgb
extern bool setup();

// make YUV image
extern bool makeYUVImage();

// make RGB image
extern int makeRGBImage(void (*convert)(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height), int numOfThread);

// YUV convert to RGB
extern void convertYUVtoRGB(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);

#if QTB_SIMD_SUPPORT
// YUV convert to RGB (SIMD version)
extern void convertYUVtoRGB_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#endif // QTB_SIMD_SUPPORT

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // YUV2RGB_H
