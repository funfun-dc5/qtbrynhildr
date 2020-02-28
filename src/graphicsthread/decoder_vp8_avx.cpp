// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_avx.h"

namespace qtbrynhildr {

// constructor
DecoderVP8AVX::DecoderVP8AVX(QImage *image)
  :DecoderVP8(image)
{
}

// destructor
DecoderVP8AVX::~DecoderVP8AVX()
{
}

#if 0
// pre-process
bool DecoderVP8AVX::preprocess(char *buffer, int size)
{
  cout << "DecoderVP8AVX::preprocess()" << endl << flush;

  return true;
}
#endif // 0

// get desktop image
QImage* DecoderVP8AVX::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderVP8AVX::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_AVX, numOfThread); // Yet

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
