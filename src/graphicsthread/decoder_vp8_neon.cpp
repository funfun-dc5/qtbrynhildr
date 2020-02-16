// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8_neon.h"

namespace qtbrynhildr {

// constructor
DecoderVP8NEON::DecoderVP8NEON(QImage *image)
  :DecoderVP8(image)
  // for DEBUG
  //,outputLog(false)
{
}

// destructor
DecoderVP8NEON::~DecoderVP8NEON()
{
}

#if 0
// pre-process
bool DecoderVP8NEON::preprocess(char *buffer, int size)
{
  cout << "DecoderVP8NEON::preprocess()" << endl << flush;

  return true;
}
#endif // 0

// get desktop image
QImage* DecoderVP8NEON::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderVP8NEON::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_NEON, numOfThread); // Yet

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
