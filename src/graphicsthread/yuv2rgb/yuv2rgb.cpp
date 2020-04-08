// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018-2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

// Qt Header
#if QTB_MULTI_THREAD_CONVERTER
#include <QtConcurrent>
#endif // QTB_MULTI_THREAD_CONVERTER

// Local Header
#include "bitmap.h"
#include "yuv2rgb.h"

namespace qtbrynhildr {

#if QTB_BENCHMARK
// calculation rate
double calcRate = 0.0;
#endif // QTB_BENCHMARK

// parameters
int width = 0;
int height = 0;
int rgbImageSize = 0;
int uvNext = 0;
int rgbNext = 0;

uchar *yuv = 0;
Aligned(16) uchar *yuv1 = 0;
Aligned(16) uchar *yuv2 = 0;
uchar *y1topOrg = 0;
uchar *u1topOrg = 0;
uchar *v1topOrg = 0;
uchar *y2topOrg = 0;
uchar *u2topOrg = 0;
uchar *v2topOrg = 0;

#if QTB_MULTI_THREAD_CONVERTER
#define ALL_NEW_THREAD 0
#if ALL_NEW_THREAD
// for thread wait
QFuture<void> f1, f2, f3, f4;
#else // ALL_NEW_THREAD
QFuture<void> f1, f2, f3;
#endif // ALL_NEW_THREAD
#endif // QTB_MULTI_THREAD_CONVERTER

#if QTB_LOAD_BITMAP
Aligned(16) uchar *bmp = 0;
BITMAPFILEHEADER *bfHeader = 0;
BITMAPINFOHEADER *biHeader = 0;
#endif // QTB_LOAD_BITMAP
Aligned(16) uchar *rgb = 0;

#if !defined(BENCHMARK)
// codec context
vpx_codec_ctx_t c_codec;

// initialize for yuv
void initVPX()
{
  // initialize libvpx
  memset(&c_codec, 0, sizeof(c_codec)); // for coverity scan
  vpx_codec_dec_init(&c_codec, &vpx_codec_vp8_dx_algo, 0, 0);
}

// decode VP8
void decodeVPX(uchar *buffer, int size)
{
  // decode vp8
  vpx_codec_decode(&c_codec, (uint8_t*)buffer, size, 0, 0);
}
#endif // !defined(BENCHMARK)

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
  memset(yuv2, 0, yuvImageSize);

  int size = width * height;
  rgbImageSize = size * IMAGE_FORMAT_SIZE;
#if QTB_LOAD_BITMAP
  if (sizeof(BITMAPFILEHEADER) != 14 ||
	  sizeof(BITMAPINFOHEADER) != 40){
	cout << "[bitmap] sizeof(BITMAPFILEHEADER) = " << sizeof(BITMAPFILEHEADER) << endl << flush;
    cout << "[bitmap] sizeof(BITMAPINFOHEADER) = " << sizeof(BITMAPINFOHEADER) << endl << flush;
    cout << "[bitmap] sizeof(RGBQUAD) = " << sizeof(RGBQUAD) << endl << flush;
	return false;
  }
  int headerSize = 64; // sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 10 (16 bytes boundary)
  if (bmp != 0){
	delete [] bmp;
  }
  bmp = new uchar[headerSize + rgbImageSize];
  rgb = bmp + headerSize; // rgb image top address
  bfHeader = (BITMAPFILEHEADER*)bmp;
  biHeader = (BITMAPINFOHEADER*)(bmp + sizeof(BITMAPFILEHEADER));
  // initialize BITMAPFILEHEADER
  bfHeader->bfType[0] = 'B';
  bfHeader->bfType[1] = 'M';
  bfHeader->bfSize = rgbImageSize + headerSize;
  bfHeader->bfReserved1 = 0;
  bfHeader->bfReserved2 = 0;
  bfHeader->bfOffBits = 64;
  // initialize BITMAPINFOHEADER
  biHeader->biSize = BI_SIZE;
  biHeader->biWidth = width;
  biHeader->biHeight = height;
  biHeader->biPlanes = 1;
  biHeader->biBitCount = 32;
  biHeader->biCompression = BI_RGB;
  biHeader->biSizeImage = 0;
  biHeader->biXPixPerMeter = 0;
  biHeader->biYPixPerMeter = 0;
  biHeader->biClrUsed = 0;
  biHeader->biCirImportant = 0;
#else // QTB_LOAD_BITMAP
  if (rgb != 0){
	delete [] rgb;
  }
  rgb = new uchar[rgbImageSize];
#endif // QTB_LOAD_BITMAP
  memset(rgb, 0, rgbImageSize);
  for(int i = 3; i < rgbImageSize; i += 4){
	rgb[i] = 0xff;
  }

  // cout << "yuv1 address = " << hex << (int*)yuv1 << endl;
  // cout << "yuv2 address = " << hex << (int*)yuv2 << endl;
  // cout << "rgb address = " << hex << (int*)rgb << endl;

  // calc parameters
  y1topOrg = (uchar*)yuv1;
  u1topOrg = y1topOrg + size;
  v1topOrg = u1topOrg + size / 4;
  y2topOrg = (uchar*)yuv2;
  u2topOrg = y2topOrg + size;
  v2topOrg = u2topOrg + size / 4;
  yuv = yuv1;
  uvNext = width / 2;
#if !QTB_LOAD_BITMAP
  rgbNext = - width * IMAGE_FORMAT_SIZE * 2;
#endif // !QTB_LOAD_BITMAP

  return true;
}

#if !defined(BENCHMARK)
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
#else // !defined(BENCHMARK)
// make YUV image
bool makeYUVImage()
{
  return true;
}
#endif // !defined(BENCHMARK)

int makeRGBImage(void (*convert)(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height), int numOfThread)
{
  // make yuv image
  if (!makeYUVImage()){
	return 0;
  }

  // number of thread 1 or 2 or 4
#if QTB_LOAD_BITMAP
  uchar *rgbtop = rgb;
#else // QTB_LOAD_BITMAP
  uchar *rgbtop = rgb + width * (height - 1) * IMAGE_FORMAT_SIZE;
#endif // QTB_LOAD_BITMAP
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

#if QTB_MULTI_THREAD_CONVERTER
#if ALL_NEW_THREAD
  // other threads
  int linesOfThread = height / numOfThread;
#if QTB_LOAD_BITMAP
  int rgbtopNextThread = width * linesOfThread * IMAGE_FORMAT_SIZE;
#else // QTB_LOAD_BITMAP
  int rgbtopNextThread = - (width * linesOfThread) * IMAGE_FORMAT_SIZE;
#endif // QTB_LOAD_BITMAP
  int ytopNextThread = width * linesOfThread;
  int uvtopNextThread = uvNext * linesOfThread/2;

  // start 1st thread
  f1 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);

  if (numOfThread >= 2){
	// for next thread
	rgbtop += rgbtopNextThread;
	ytop += ytopNextThread;
	utop += uvtopNextThread;
	vtop += uvtopNextThread;

	// start 2nd thread
	f2 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);
  }
  if (numOfThread >= 3){
	// for next thread
	rgbtop += rgbtopNextThread;
	ytop += ytopNextThread;
	utop += uvtopNextThread;
	vtop += uvtopNextThread;

	// start 3rd thread
	f3 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);
  }
  if (numOfThread >= 4){
	// for next thread
	rgbtop += rgbtopNextThread;
	ytop += ytopNextThread;
	utop += uvtopNextThread;
	vtop += uvtopNextThread;

	// start 4th thread
	f4 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);
  }

  // wait for all threads finished
  f1.waitForFinished();
  f2.waitForFinished();
  f3.waitForFinished();
  f4.waitForFinished();

#else // ALL_NEW_THREAD

  // this thread and other threads
  if (numOfThread <= 1 || height % 2 != 0){
	(*convert)(ytop, utop, vtop, rgbtop, height);
  }
  else { // numOfThread >= 2
	int linesOfThread = height / numOfThread;
#if QTB_LOAD_BITMAP
	int rgbtopNextThread = width * linesOfThread * IMAGE_FORMAT_SIZE;
#else // QTB_LOAD_BITMAP
	int rgbtopNextThread = - (width * linesOfThread) * IMAGE_FORMAT_SIZE;
#endif // QTB_LOAD_BITMAP
	int ytopNextThread = width * linesOfThread;
	int uvtopNextThread = uvNext * linesOfThread/2;

	// start 1st thread
	f1 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);

	if (numOfThread >= 3){
	  // for next thread
	  //rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  rgbtop += rgbtopNextThread;
	  ytop += ytopNextThread;
	  utop += uvtopNextThread;
	  vtop += uvtopNextThread;

	  // start 2nd thread
	  f2 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);
	}
	if (numOfThread >= 4){
	  // for next thread
	  //rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  rgbtop += rgbtopNextThread;
	  ytop += ytopNextThread;
	  utop += uvtopNextThread;
	  vtop += uvtopNextThread;

	  // start 3rd thread
	  f3 = QtConcurrent::run(convert, ytop, utop, vtop, rgbtop, linesOfThread);
	}

	// start last thread (this thread)
	//rgbtop -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	rgbtop += rgbtopNextThread;
	ytop += ytopNextThread;
	utop += uvtopNextThread;
	vtop += uvtopNextThread;
	(*convert)(ytop, utop, vtop, rgbtop, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();
  }

#endif // ALL_NEW_THREAD
#else // QTB_MULTI_THREAD_CONVERTER
  // no other thread
  Q_UNUSED(numOfThread);
  (*convert)(ytop, utop, vtop, rgbtop, height);
#endif // QTB_MULTI_THREAD_CONVERTER

#if 0 // for TEST
  {
	static bool doFlag = true;
	static int counter = 0;
	counter++;
	if (doFlag && counter == 30){
	  fstream file;
	  int yuvImageSize = width * height + width * height / 2;

	  file.open("jpg/rgb.dat", ios::out | ios::binary);
	  if (file.is_open()){
		file.write((const char*)rgb, rgbImageSize);
		file.close();
	  }
	  file.open("jpg/yuv.dat", ios::out | ios::binary);
	  if (file.is_open()){
		file.write((const char*)yuv, yuvImageSize);
		file.close();
	  }
	  doFlag = false;
	}
  }
#endif // for TEST

  return rgbImageSize;
}

} // end of namespace qtbrynhildr
