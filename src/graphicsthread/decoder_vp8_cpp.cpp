// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_cpp.h"

namespace qtbrynhildr {

// constructor
DecoderVP8CPP::DecoderVP8CPP()
  :DecoderVP8()
{
}

// destructor
DecoderVP8CPP::~DecoderVP8CPP()
{
}

// get desktop image
QImage* DecoderVP8CPP::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  std::cout << "DecoderVP8CPP::getDesktopImage()" << std::endl << std::flush;
	  flag = false;
	}
  }

  // make RGB image
#if QTB_GRAY_SCALE_MODE
  int rgbImageSize = 0;
  if (onGrayScale){
	// gray scale
	rgbImageSize = makeRGBImage(convertYUVtoRGB_CPP_GSV, numOfThread);
  }
  else {
	// full color
	rgbImageSize = makeRGBImage(convertYUVtoRGB_CPP, numOfThread);
  }
#else // QTB_GRAY_SCALE_MODE
  // full color
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_CPP, numOfThread);
#endif // QTB_GRAY_SCALE_MODE

#if QTB_GRAY_SCALE_MODE2
  // RGB convert to GS
  convertRGBtoGS();
#endif // QTB_GRAY_SCALE_MODE2

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
