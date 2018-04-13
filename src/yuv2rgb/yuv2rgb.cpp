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
int uvNext = 0;
int rgb24Next = 0;

uchar *yuv420 = 0;
uchar *yuv1 = 0;
uchar *y1topOrg = 0;
uchar *u1topOrg = 0;
uchar *v1topOrg = 0;
uchar *y2topOrg = 0;
uchar *u2topOrg = 0;
uchar *v2topOrg = 0;

#endif // QTB_MULTI_THREAD_CONVERTER

// setup for yuv420, rgb24
inline bool QtBrynhildr::setup()
{
  int yuvBufferSize = width * height + width * height / 2;
  // allocate yuv420/rgb24 buffer
  if (yuv1 != 0){
	delete [] yuv1;
  }
  yuv1 = new uchar[yuvBufferSize];
  memset(yuv1, 0, yuvBufferSize);
  if (yuv2 != 0){
	delete [] yuv2;
  }
  yuv2 = new uchar[yuvBufferSize];
  memset(yuv2, 0xFF, yuvBufferSize);
#if USE_PPM_LOADER_FOR_VP8
  if (ppm != 0){
	delete [] ppm;
  }
  ppm = new uchar[width * height * 3 + PPM_HEADER_SIZE_MAX];
  // make PPM header
  int length = snprintf((char*)ppm, PPM_HEADER_SIZE_MAX, PPM_HEADER_FORMAT, width, height);
  // set rgb24
  rgb24 = ppm + length;
#else // USE_PPM_LOADER_FOR_VP8
  if (rgb24 != 0){
	delete [] rgb24;
  }
  rgb24 = new uchar[width * height * IMAGE_FORMAT_SIZE];
#endif // USE_PPM_LOADER_FOR_VP8

  // calc parameters
  hwidth = width / 2;
  size = width * height;
  y1topOrg = (uchar*)yuv1;
  u1topOrg = y1topOrg + size;
  v1topOrg = u1topOrg + size / 4;
  y2topOrg = (uchar*)yuv2;
  u2topOrg = y2topOrg + size;
  v2topOrg = u2topOrg + size / 4;
  yuv420 = yuv1;
  uvNext = width / 2;
  rgb24Next = - width * IMAGE_FORMAT_SIZE * 2;
#if QTB_MULTI_THREAD_CONVERTER
  // set for qtbrynhhildr::convertYUV420toRGB24()
  qtbrynhildr::width = width;
  qtbrynhildr::uvNext = uvNext;
  qtbrynhildr::rgb24Next = rgb24Next;
  qtbrynhildr::yuv420 = yuv420;
  qtbrynhildr::yuv1 = yuv1;
  qtbrynhildr::y1topOrg = y1topOrg;
  qtbrynhildr::u1topOrg = u1topOrg;
  qtbrynhildr::v1topOrg = v1topOrg;
  qtbrynhildr::y2topOrg = y2topOrg;
  qtbrynhildr::u2topOrg = u2topOrg;
  qtbrynhildr::v2topOrg = v2topOrg;
#endif // QTB_MULTI_THREAD_CONVERTER

  return true;
}

// make YUV420 image
inline bool QtBrynhildr::makeYUV420Image()
{
  // get 1 frame image (YUV420)
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

	// setup for yuv420, rgb24
	if (!setup()){
	  return false;
	}
  }

  // switch yuv420 buffer
  if (yuv420 == yuv1){
	yuv420 = yuv2;
  }
  else {
	yuv420 = yuv1;
  }
  qtbrynhildr::yuv420 = yuv420;

  // create yuv420
  uchar *top = yuv420;

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

// make RGB24 image
inline int QtBrynhildr::makeRGB24Image()
{
  // make yuv420 image
  if (!makeYUV420Image()){
	return 0;
  }

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread 1 or 2 or 4
  int numOfThread = settings->getConvertThreadCount();
  uchar *rgb24top = rgb24 + width * (height - 1) * IMAGE_FORMAT_SIZE;
  uchar *ytop;
  uchar *utop;
  uchar *vtop;
  if (yuv420 == yuv1){
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
	// convert YUV420 to RGB24
	qtbrynhildr::convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
	return size * IMAGE_FORMAT_SIZE;
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<void> f1, f2, f3;
	int linesOfThread = height / numOfThread;

	// start 1st thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);

	if (numOfThread > 2){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 2nd thread
	  f2 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for 3rd thread
	if (numOfThread > 3){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 3rd thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for next thread
	rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// for last thread
	qtbrynhildr::convertYUV420toRGB24(ytop, utop, vtop, rgb24top, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();

	return size * IMAGE_FORMAT_SIZE;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV420 to RGB24
  qtbrynhildr::convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
  return size * IMAGE_FORMAT_SIZE;
#endif // QTB_MULTI_THREAD_CONVERTER
}

#if QTB_SIMD_SUPPORT
// make RGB24 image by SIMD operation
inline int QtBrynhildr::makeRGB24Image_SIMD()
{
  // make yuv420 image
  if (!makeYUV420Image()){
	return 0;
  }

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread 1 or 2 or 4
  int numOfThread = settings->getConvertThreadCount();
  uchar *rgb24top = rgb24 + width * (height - 1) * IMAGE_FORMAT_SIZE;
  uchar *ytop;
  uchar *utop;
  uchar *vtop;
  if (yuv420 == yuv1){
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
	// convert YUV420 to RGB24
	qtbrynhildr::convertYUV420toRGB24_SIMD(ytop, utop, vtop, rgb24top, height);
	return size * IMAGE_FORMAT_SIZE;
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<void> f1, f2, f3;
	int linesOfThread = height / numOfThread;

	// start 1st thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24_SIMD, ytop, utop, vtop, rgb24top, linesOfThread);

	if (numOfThread > 2){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 2nd thread
	  f2 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24_SIMD, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for 3rd thread
	if (numOfThread > 3){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 3rd thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24_SIMD, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for next thread
	rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// for last thread
	qtbrynhildr::convertYUV420toRGB24_SIMD(ytop, utop, vtop, rgb24top, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();

	return size * IMAGE_FORMAT_SIZE;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV420 to RGB24
  qtbrynhildr::convertYUV420toRGB24_SIMD(ytop, utop, vtop, rgb24top, height);
  return size * IMAGE_FORMAT_SIZE;
#endif // QTB_MULTI_THREAD_CONVERTER
}
#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
