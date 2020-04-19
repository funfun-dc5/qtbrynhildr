// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

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

#if 0
// pre-process
bool DecoderVP8CPP::preprocess(char *buffer, int size)
{
  cout << "DecoderVP8CPP::preprocess()" << endl << flush;

  return true;
}
#endif // 0

// get desktop image
QImage* DecoderVP8CPP::getDesktopImage(int numOfThread)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderVP8CPP::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_CPP, numOfThread);

  // get QImage
  image = getImage(rgbImageSize);

  return image;
}

} // end of namespace qtbrynhildr
