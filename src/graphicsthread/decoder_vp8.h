
// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_VP8_H
#define DECODER_VP8_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header
#include "decoder.h"
#include "settings.h"
#include "yuv2rgb/yuv2rgb.h"

namespace qtbrynhildr {

class DecoderVP8 : public Decoder
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // flag for initVPX()
  static bool doneInitVPX;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DecoderVP8();
  // destructor
  virtual ~DecoderVP8();

  // get decoder name
  const char*  name()
  {
	return "vp8 decoder";
  }

  // get suffix
  const char* suffix()
  {
	return "vp8";
  }

  // pre-process
  bool preprocess(char *buffer, int size);

protected:
  // get image
  QImage* getImage(int imageSize);

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_VP8_H
