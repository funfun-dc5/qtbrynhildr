// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_VP8_AVX_H
#define DECODER_VP8_AVX_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header
#include "decoder_vp8.h"

namespace qtbrynhildr {

class DecoderVP8AVX : public DecoderVP8
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
  DecoderVP8AVX();
  // destructor
  virtual ~DecoderVP8AVX();

  // get decoder name
  const char*  name()
  {
	return "AVX";
  }

  // get desktop image
  QImage* getDesktopImage(int numOfThread);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_VP8_AVX_H
