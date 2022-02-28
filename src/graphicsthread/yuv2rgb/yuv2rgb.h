// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018- FunFun <fu.aba.dc5@gmail.com>

#ifndef YUV2RGB_H
#define YUV2RGB_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


// libvxp Header
#if !defined(BENCHMARK)
#include "vpx_decoder.h"
#include "vp8dx.h"
#endif // !defined(BENCHMARK)

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

// for benchmark
#if QTB_BENCHMARK
// calculation rate
extern double calcRate;
#endif //QTB_BENCHMARK

// parameters
extern int width;
extern int height;
extern int imageSize;
extern int uvNext;
#if !QTB_LOAD_BITMAP
extern int rgbNext;
#endif // !QTB_LOAD_BITMAP

extern uchar *yuv;
extern uchar *yuv1;
extern uchar *yuv2;
extern uchar *y1topOrg;
extern uchar *u1topOrg;
extern uchar *v1topOrg;
extern uchar *y2topOrg;
extern uchar *u2topOrg;
extern uchar *v2topOrg;

#if QTB_LOAD_BITMAP
extern uchar *bmp;
#endif // QTB_LOAD_BITMAP
extern uchar *rgb;

#if !defined(BENCHMARK)
// codec context
extern vpx_codec_ctx_t c_codec;

// initialize for yuv
extern void initVPX();

// uninitialize for yuv
extern void uninitVPX();

// decode VP8
extern void decodeVPX(uchar *buffer, int size);
#endif // !defined(BENCHMARK)

// initialize for yuv, rgb
extern void initYUV2RGB();

// setup for yuv, rgb
extern bool setup();

// make YUV image
extern bool makeYUVImage();

// make RGB image
extern int makeRGBImage(void (*convert)(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height), int numOfThread);

// YUV convert to RGB
extern void convertYUVtoRGB_CPP(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);

#if QTB_SIMD_SUPPORT
#if !(defined(__arm__) || defined(__aarch64__) || defined(Q_OS_IOS))
// YUV convert to RGB (SIMD version)
extern void convertYUVtoRGB_SIMD_SSE(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
extern void convertYUVtoRGB_SIMD_AVX(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
extern void convertYUVtoRGB_SIMD_AVX2(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#else // !(defined(__arm__) || defined(__aarch64__))
extern void convertYUVtoRGB_SIMD_NEON(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#endif // !(defined(__arm__) || defined(__aarch64__))
#endif // QTB_SIMD_SUPPORT

#if QTB_GRAY_SCALE_MODE
// YUV convert to RGB (Gray Scale Version)
extern void convertYUVtoRGB_CPP_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);

#if QTB_SIMD_SUPPORT
#if !(defined(__arm__) || defined(__aarch64__))
// YUV convert to RGB (SIMD version)
extern void convertYUVtoRGB_SIMD_SSE_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
extern void convertYUVtoRGB_SIMD_AVX_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
extern void convertYUVtoRGB_SIMD_AVX2_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#else // !(defined(__arm__) || defined(__aarch64__))
extern void convertYUVtoRGB_SIMD_NEON_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height);
#endif // !(defined(__arm__) || defined(__aarch64__))
#endif // QTB_SIMD_SUPPORT

// reset YUV buffer
extern void resetYUVBuffer();
#endif // QTB_GRAY_SCALE_MODE

#if QTB_GRAY_SCALE_MODE2
// RGB convert to Gray Scale
extern void convertRGBtoGS();
#endif // QTB_GRAY_SCALE_MODE2

} // end of namespace qtbrynhildr

#endif // YUV2RGB_H
