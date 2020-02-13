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
  :
  // for DEBUG
  outputLog(true)
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
  static bool flag = true;
  if (flag){
	cout << "DecoderVP8CPP::getDesktopImage()" << endl << flush;
	flag = false;
  }

#if 1 // for TEST

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB, numOfThread);

  // get QImage
  image = getImage(rgbImageSize);

  return image;

#else // 1 // for TEST

  bool result = false;

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB, numOfThread);

  if (rgbImageSize != 0){
#if QTB_LOAD_BITMAP
	// load a BMP data to QImage
	result = image->loadFromData((const uchar *)qtbrynhildr::bmp,
								 (uint)rgbImageSize + 64,
								 "BMP");
#else // QTB_LOAD_BITMAP
	// create QImage
	if (image != 0){
	  delete image;
	}
	image = new QImage(qtbrynhildr::rgb, qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
	result = true;
#endif // QTB_LOAD_BITMAP
  }
  else {
	if (image->isNull()){
	  delete image;
	  image = new QImage(qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
	  image->fill(QTB_DESKTOP_BACKGROUND_COLOR);
	}
  }

  return result ? image : nullptr;

#endif // 1 // for TEST
}

} // end of namespace qtbrynhildr
