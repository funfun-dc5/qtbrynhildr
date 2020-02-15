// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_vp8.h"

namespace qtbrynhildr {

// constructor
DecoderVP8::DecoderVP8(QImage *image)
  :Decoder(image)
  // for DEBUG
  ,outputLog(true)
{
}

// destructor
DecoderVP8::~DecoderVP8()
{
}

// pre-process
bool DecoderVP8::preprocess(char *buffer, int size)
{
  static bool flag = true;
  if (flag){
	cout << "DecoderVP8::preprocess()" << endl << flush;
	flag = false;
  }

  // set parameters
  this->buffer = buffer;
  this->size = size;

  // decode VP8
  decodeVPX((uchar*)buffer, size);

  return true;
}

#if 0
// get desktop image
QImage* DecoderVP8::getDesktopImage(int numOfThread)
{
  cout << "DecoderVP8::getDesktopImage()" << endl << flush;

  return nullptr;
}
#endif // 0

// get image
QImage* DecoderVP8::getImage(int rgbImageSize)
{
  bool result = false;

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
}

} // end of namespace qtbrynhildr
