// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>
#if defined(__x86_64__) // for Universal Binary

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_sse.h"

namespace qtbrynhildr {

// constructor
DecoderVP8SSE::DecoderVP8SSE()
  :DecoderVP8()
{
}

// destructor
DecoderVP8SSE::~DecoderVP8SSE()
{
}

// get desktop image
QImage* DecoderVP8SSE::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  std::cout << "DecoderVP8SSE::getDesktopImage()" << std::endl << std::flush;
	  flag = false;
	}
  }

  // make RGB image
#if QTB_GRAY_SCALE_MODE
  int rgbImageSize = 0;
  if (onGrayScale){
	// gray scale
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_SSE_GSV, numOfThread);
  }
  else {
	// full color
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_SSE, numOfThread);
  }
#else // !QTB_GRAY_SCALE_MODE
  // full color
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_SSE, numOfThread);
#endif // !QTB_GRAY_SCALE_MODE

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr

#endif // defined(__x86_64__) // for Universal Binary
