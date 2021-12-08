// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_JPEG_H
#define DECODER_JPEG_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header
#include "decoder.h"

namespace qtbrynhildr {

class DecoderJPEG : public Decoder
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DecoderJPEG();
  // destructor
  virtual ~DecoderJPEG();

  // get decoder name
  const char* name()
  {
	return "JPEG";
  }

  // get suffix
  const char* suffix()
  {
	return "jpg";
  }

  // get desktop image
  QImage* getDesktopImage(int numOfThread);

  // set gray scale flag
  virtual void setOnGrayScale(bool onGrayScale);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_JPEG_H
