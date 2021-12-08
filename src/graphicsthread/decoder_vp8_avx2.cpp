// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_avx2.h"

namespace qtbrynhildr {

// constructor
DecoderVP8AVX2::DecoderVP8AVX2()
  :DecoderVP8()
{
}

// destructor
DecoderVP8AVX2::~DecoderVP8AVX2()
{
}

// get desktop image
QImage* DecoderVP8AVX2::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderVP8AVX2::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // make RGB image
#if QTB_GRAY_SCALE_MODE
  int rgbImageSize = 0;
  if (onGrayScale){
	// gray scale
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_AVX2_GSV, numOfThread);
  }
  else {
	// full color
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_AVX2, numOfThread);
  }
#else // QTB_GRAY_SCALE_MODE
  // full color
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_AVX2, numOfThread);
#endif // QTB_GRAY_SCALE_MODE

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
