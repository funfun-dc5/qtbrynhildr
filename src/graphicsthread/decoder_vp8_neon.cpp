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
  ,outputLog(true)
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
  static bool flag = true;
  if (flag){
	cout << "DecoderVP8NEON::getDesktopImage()" << endl << flush;
	flag = false;
  }

#if 1 // for TEST

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD_NEON, numOfThread); // Yet

  // get QImage
  image = getImage(rgbImageSize);

  return image;

#else // 1 // for TEST

  bool result = false;

  // make RGB image
  int rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD, numOfThread);

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
