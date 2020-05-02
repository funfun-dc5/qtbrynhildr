// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_VP8_AVX2_H
#define DECODER_VP8_AVX2_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header
#include "decoder_vp8.h"

namespace qtbrynhildr {

class DecoderVP8AVX2 : public DecoderVP8
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
  DecoderVP8AVX2();
  // destructor
  virtual ~DecoderVP8AVX2();

  // get decoder name
  const char*  name()
  {
	return "SSE+AVX2";
  }

#if 0
  // pre-process
  bool preprocess(char *buffer, int size);
#endif // 0

  // get desktop image
  QImage* getDesktopImage(int numOfThread);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_VP8_AVX2_H
