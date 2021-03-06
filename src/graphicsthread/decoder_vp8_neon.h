// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_VP8_NEON_H
#define DECODER_VP8_NEON_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header
#include "decoder_vp8.h"

namespace qtbrynhildr {

class DecoderVP8NEON : public DecoderVP8
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
  DecoderVP8NEON();
  // destructor
  virtual ~DecoderVP8NEON();

  // get decoder name
  const char*  name()
  {
	return "NEON";
  }

  // get desktop image
  QImage* getDesktopImage(int numOfThread);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_VP8_NEON_H
