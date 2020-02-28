// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header

// System Header

// Qt Header
#include <QApplication>
#include <QtConcurrent>

// Local Header
#include "yuv2rgb.h"

int convertFrame()
{
#if defined(__AVX2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_AVX2, MULTI_THREAD);
#elif defined(__AVX__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_AVX, MULTI_THREAD);
#elif defined(__SSE4_2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_SIMD_SSE, MULTI_THREAD);
#else // defined(__SSE4_2__)
  return qtbrynhildr::makeRGBImage(qtbrynhildr::convertYUVtoRGB_CPP, MULTI_THREAD);
#endif // defined(__SSE4_2__)
}

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // benchmark spec.
  qtbrynhildr::width = 1024;
  qtbrynhildr::height = 800;

  // setup
  qtbrynhildr::setup();
  int yuvImageSize = qtbrynhildr::width * qtbrynhildr::height + qtbrynhildr::width * qtbrynhildr::height / 2;
  memset(qtbrynhildr::yuv1, 0xff, yuvImageSize);

  // execute LOOP_COUNT times
  for(int i = 0; i < LOOP_COUNT; i++){
	int imageSize = convertFrame();
	if (imageSize == 0){
	  cout << "Error : imageSize == 0" << endl << flush;
	}
  }

  exit(0);
}
