// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

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

#if 0
// pre-process
bool DecoderVP8SSE::preprocess(char *buffer, int size)
{
  cout << "DecoderVP8SSE::preprocess()" << endl << flush;

  return true;
}
#endif // 0

// get desktop image
QImage* DecoderVP8SSE::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderVP8SSE::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_SSE, numOfThread);

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
