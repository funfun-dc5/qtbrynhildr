// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cmath>
#if QTB_PUBLIC_MODE7_SUPPORT
#if USE_PPM_LOADER_FOR_VP8
#include <cstdio>
#endif // USE_PPM_LOADER_FOR_VP8
#include <cstring>
#endif // QTB_PUBLIC_MODE7_SUPPORT
#include <fstream>
#include <iostream>

// Qt Header
#if QTB_MULTI_THREAD_CONVERTER
#include <QtConcurrent>
#endif // QTB_MULTI_THREAD_CONVERTER

// Local Header
#include "yuv2rgb.h"

#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

#if QTB_MULTI_THREAD_CONVERTER

// parameters
int width = 0;
int height = 0;
int rgbImageSize = 0;
int hwidth = 0;
int uvNext = 0;
int rgbNext = 0;

uchar *yuv = 0;
uchar *yuv1 = 0;
uchar *yuv2 = 0;
uchar *y1topOrg = 0;
uchar *u1topOrg = 0;
uchar *v1topOrg = 0;
uchar *y2topOrg = 0;
uchar *u2topOrg = 0;
uchar *v2topOrg = 0;

uchar *rgb = 0;

// codec context
vpx_codec_ctx_t c_codec;

#endif // QTB_MULTI_THREAD_CONVERTER

// initialize for yuv
void initVPX()
{
#if QTB_PUBLIC_MODE7_SUPPORT
  // initialize libvpx
  memset(&c_codec, 0, sizeof(c_codec)); // for coverity scan
  vpx_codec_dec_init(&c_codec, &vpx_codec_vp8_dx_algo, 0, 0);
#endif // QTB_PUBLIC_MODE7_SUPPORT
}

// decode VP8
void decodeVPX(uchar *buffer, int size)
{
#if QTB_PUBLIC_MODE7_SUPPORT
  // decode vp8
  //  if (com_data->video_mode == VIDEO_MODE_COMPRESS){
  vpx_codec_decode(&c_codec, (uint8_t*)buffer, size, 0, 0);
  //  }
#endif // QTB_PUBLIC_MODE7_SUPPORT
}

// setup for yuv, rgb
bool setup()
{
  int yuvImageSize = width * height + width * height / 2;
  // allocate yuv/rgb buffer
  if (yuv1 != 0){
	delete [] yuv1;
  }
  yuv1 = new uchar[yuvImageSize];
  memset(yuv1, 0, yuvImageSize);
  if (yuv2 != 0){
	delete [] yuv2;
  }
  yuv2 = new uchar[yuvImageSize];
  memset(yuv2, 0xFF, yuvImageSize);

  int size = width * height;
  rgbImageSize = size * IMAGE_FORMAT_SIZE;
#if USE_PPM_LOADER_FOR_VP8
  if (ppm != 0){
	delete [] ppm;
  }
  ppm = new uchar[rgbImageSize + PPM_HEADER_SIZE_MAX];
  // make PPM header
  int length = snprintf((char*)ppm, PPM_HEADER_SIZE_MAX, PPM_HEADER_FORMAT, width, height);
  // set rgb
  rgb = ppm + length;
#else // USE_PPM_LOADER_FOR_VP8
  if (rgb != 0){
	delete [] rgb;
  }
  rgb = new uchar[rgbImageSize];
#endif // USE_PPM_LOADER_FOR_VP8

  // calc parameters
  y1topOrg = (uchar*)yuv1;
  u1topOrg = y1topOrg + size;
  v1topOrg = u1topOrg + size / 4;
  y2topOrg = (uchar*)yuv2;
  u2topOrg = y2topOrg + size;
  v2topOrg = u2topOrg + size / 4;
  yuv = yuv1;
  uvNext = width / 2;
  rgbNext = - width * IMAGE_FORMAT_SIZE * 2;

  return true;
}

// make YUV image
bool makeYUVImage()
{
  // half of width
  static int hwidth = 0;

  // get 1 frame image (YUV)
  vpx_codec_iter_t iter = 0;
  vpx_image_t *img = vpx_codec_get_frame(&c_codec, &iter);
  if (img == 0){
	return false;
  }

  // set size
  if (width  != (int)img->d_w ||
	  height != (int)img->d_h){
	// set new width/height
	width  = img->d_w;
	height = img->d_h;
    hwidth = width / 2;

	// setup for yuv, rgb
	if (!setup()){
	  return false;
	}
  }

  // switch yuv buffer
  if (yuv == yuv1){
	yuv = yuv2;
  }
  else {
	yuv = yuv1;
  }

  // create yuv
  uchar *top = yuv;

  // Y
  uchar *buf = img->planes[0];
  int stride = img->stride[0];
  for(int yPos = 0; yPos < height; yPos++){
	memcpy(top, buf, width);
	top += width;
	buf += stride;
  }
  // U
  buf = img->planes[1];
  stride = img->stride[1];
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, hwidth);
	top += hwidth;
	buf += stride;
  }
  // V
  buf = img->planes[2];
  stride = img->stride[2];
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, hwidth);
	top += hwidth;
	buf += stride;
  }

  return true;
}

// make RGB image
int makeRGBImage(int numOfThread)
{
  // make yuv image
  if (!makeYUVImage()){
	return 0;
  }

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread 1 or 2 or 4
  uchar *rgbtop = rgb + width * (height - 1) * IMAGE_FORMAT_SIZE;
  uchar *ytop;
  uchar *utop;
  uchar *vtop;
  if (yuv == yuv1){
	ytop = y1topOrg;
	utop = u1topOrg;
	vtop = v1topOrg;
  }
  else {
	ytop = y2topOrg;
	utop = u2topOrg;
	vtop = v2topOrg;
  }
  // 1 thread version
  if (numOfThread <= 1 || height % 2 != 0){
	// convert YUV to RGB
	qtbrynhildr::convertYUVtoRGB(ytop, utop, vtop, rgbtop, height);
	return rgbImageSize;
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<void> f1, f2, f3;
	int linesOfThread = height / numOfThread;

	// start 1st thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB, ytop, utop, vtop, rgbtop, linesOfThread);

	if (numOfThread > 2){
	  // for next thread
	  rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 2nd thread
	  f2 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB, ytop, utop, vtop, rgbtop, linesOfThread);
	}

	// for 3rd thread
	if (numOfThread > 3){
	  // for next thread
	  rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 3rd thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB, ytop, utop, vtop, rgbtop, linesOfThread);
	}

	// for next thread
	rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// for last thread
	qtbrynhildr::convertYUVtoRGB(ytop, utop, vtop, rgbtop, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();

	return rgbImageSize;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV to RGB
  qtbrynhildr::convertYUVtoRGB(ytop, utop, vtop, rgbtop, height);
  return rgbImageSize;
#endif // QTB_MULTI_THREAD_CONVERTER
}

#if QTB_SIMD_SUPPORT
// make RGB image by SIMD operation
int makeRGBImage_SIMD(int numOfThread)
{
  // make yuv image
  if (!makeYUVImage()){
	return 0;
  }

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread 1 or 2 or 4
  uchar *rgbtop = rgb + width * (height - 1) * IMAGE_FORMAT_SIZE;
  uchar *ytop;
  uchar *utop;
  uchar *vtop;
  if (yuv == yuv1){
	ytop = y1topOrg;
	utop = u1topOrg;
	vtop = v1topOrg;
  }
  else {
	ytop = y2topOrg;
	utop = u2topOrg;
	vtop = v2topOrg;
  }
  // 1 thread version
  if (numOfThread <= 1 || height % 2 != 0){
	// convert YUV to RGB
	qtbrynhildr::convertYUVtoRGB_SIMD(ytop, utop, vtop, rgbtop, height);
	return rgbImageSize;
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<void> f1, f2, f3;
	int linesOfThread = height / numOfThread;

	// start 1st thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB_SIMD, ytop, utop, vtop, rgbtop, linesOfThread);

	if (numOfThread > 2){
	  // for next thread
	  rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 2nd thread
	  f2 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB_SIMD, ytop, utop, vtop, rgbtop, linesOfThread);
	}

	// for 3rd thread
	if (numOfThread > 3){
	  // for next thread
	  rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 3rd thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUVtoRGB_SIMD, ytop, utop, vtop, rgbtop, linesOfThread);
	}

	// for next thread
	rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// for last thread
	qtbrynhildr::convertYUVtoRGB_SIMD(ytop, utop, vtop, rgbtop, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();

	return rgbImageSize;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV to RGB
  qtbrynhildr::convertYUVtoRGB_SIMD(ytop, utop, vtop, rgbtop, height);
  return rgbImageSize;
#endif // QTB_MULTI_THREAD_CONVERTER
}
#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
