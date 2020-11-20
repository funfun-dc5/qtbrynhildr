// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header

// Qt Header
#include <QApplication>
#include <QImage>
#include <QtConcurrent>

// Local Header
#include "yuv2rgb.h"

#if BENCHMARK_YUV2RGB
int convertFrame()
{
#if !defined(__ARM_NEON__)
#if defined(__AVX2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_AVX2, MULTI_THREAD);
#elif defined(__AVX__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_AVX, MULTI_THREAD);
#elif defined(__SSE4_2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_SSE, MULTI_THREAD);
#else // defined(__SSE4_2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_CPP, MULTI_THREAD);
#endif // defined(__SSE4_2__)
#else // !defined(__ARM_NEON__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_NEON, MULTI_THREAD);
#endif // !defined(__ARM_NEON__)
}
#endif // BENCHMARK_YUV2RGB

#if BENCHMARK_CREATE_IMAGE
void createImage()
{
  static QImage *image = nullptr;
  // create QImage
  if (image != nullptr){
	delete image;
  }
  image = new QImage(qtbrynhildr::rgb, qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
  int width = image->width();
}
#endif // BENCHMARK_CREATE_IMAGE

#if BENCHMARK_LOAD_IMAGE
void loadImage(int rgbImageSize)
{
  static QImage *image = new QImage();
  // load a BMP data to QImage
  bool result = image->loadFromData((const uchar *)qtbrynhildr::bmp,
									(uint)rgbImageSize + 64,
									"BMP");
  if (!result){
	// load error : Yet
  }
}
#endif // BENCHMARK_LOAD_IMAGE

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // benchmark spec.
  qtbrynhildr::width = 1280;
  qtbrynhildr::height = 800;
#if BENCHMARK_LOAD_IMAGE
  int rgbImageSize = qtbrynhildr::width * qtbrynhildr::height * IMAGE_FORMAT_SIZE;
#endif // BENCHMARK_LOAD_IMAGE

  // setup
  qtbrynhildr::setup();
  int yuvImageSize = qtbrynhildr::width * qtbrynhildr::height + qtbrynhildr::width * qtbrynhildr::height / 2;
  memset(qtbrynhildr::yuv1, 0xff, yuvImageSize);

  // execute LOOP_COUNT times
  for(int i = 0; i < LOOP_COUNT; i++){
#if BENCHMARK_CREATE_IMAGE
	createImage();
#elif BENCHMARK_LOAD_IMAGE
	loadImage(rgbImageSize);
#else // BENCHMARK_LOAD_IMAGE
	int imageSize = convertFrame();
	if (imageSize == 0){
	  cout << "Error : imageSize == 0" << endl << flush;
	  break;
	}
#endif // BENCHMARK_LOAD_IMAGE
  }

  exit(0);
}
