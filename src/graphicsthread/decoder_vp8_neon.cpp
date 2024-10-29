// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>
#if defined(__aarch64__) // for Universal Binary

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_neon.h"

namespace qtbrynhildr {

// constructor
DecoderVP8NEON::DecoderVP8NEON()
  :DecoderVP8()
{
}

// destructor
DecoderVP8NEON::~DecoderVP8NEON()
{
}

// get desktop image
QImage* DecoderVP8NEON::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  std::cout << "DecoderVP8NEON::getDesktopImage()" << std::endl << std::flush;
	  flag = false;
	}
  }

  // make RGB image
#if QTB_GRAY_SCALE_MODE
  int rgbImageSize = 0;
  if (onGrayScale){
	// gray scale
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_NEON_GSV, numOfThread);
  }
  else {
	// full color
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_NEON, numOfThread);
  }
#else // !QTB_GRAY_SCALE_MODE
  // full color
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_NEON, numOfThread);
#endif // !QTB_GRAY_SCALE_MODE

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr

#endif // defined(__aarch64__)
