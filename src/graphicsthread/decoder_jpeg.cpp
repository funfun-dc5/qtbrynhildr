// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "decoder_jpeg.h"

namespace qtbrynhildr {

// constructor
DecoderJPEG::DecoderJPEG()
  :Decoder()
{
}

// destructor
DecoderJPEG::~DecoderJPEG()
{
}

// get desktop image
QImage* DecoderJPEG::getDesktopImage(int numOfThread)
{
  Q_UNUSED(numOfThread);

  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "DecoderJPEG::getDesktopImage()" << endl << flush;
	  flag = false;
	}
  }

  // load a JPEG data to QImage
  bool result = image->loadFromData((const uchar *)buffer,
									(uint)size,
									"JPEG");
  // return
  return result ? image : nullptr;
}

} // end of namespace qtbrynhildr
