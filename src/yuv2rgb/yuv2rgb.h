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

// use loadFromData() instead of new QImage()
#define USE_PPM_LOADER_FOR_VP8	0

#if USE_PPM_LOADER_FOR_VP8
// for PPM Header
#define PPM_HEADER_SIZE_MAX 32
// "P6\n"            : PPM binary
// "wwww hhhh\n"     : width height
// "255\n"           : max value (255)
#define PPM_HEADER_FORMAT "P6\n%d %d\n255\n"
#endif // USE_PPM_LOADER_FOR_VP8

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
#if USE_PPM_LOADER_FOR_VP8
extern uchar *ppm;
#endif // USE_PPM_LOADER_FOR_VP8

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
extern int makeRGBImage(int numOfThread);

#if QTB_SIMD_SUPPORT
// make RGB image by SIMD operation
extern int makeRGBImage_SIMD(int numOfThread);
#endif // QTB_SIMD_SUPPORT

// YUV convert to RGB
extern void convertYUVtoRGB(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);

#if QTB_SIMD_SUPPORT
// YUV convert to RGB (SIMD version)
extern void convertYUVtoRGB_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#endif // QTB_SIMD_SUPPORT

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // YUV2RGB_H
