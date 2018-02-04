// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef CONVERTER_OPUS_H
#define CONVERTER_OPUS_H

// Qt Header

// CELT (opus)
#include "celt.h"

// Local Header
#include "converter.h"

namespace qtbrynhildr {

// Converter
class Converter_CELT : public Converter
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // frame size
  int frameSize;

  // mode
  OpusCustomMode *mode;

  // decorder
  OpusCustomDecoder *decoder;

  // work buffer
  char *workBuffer;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  Converter_CELT(int samplerate, int channels);
  // destructor
  ~Converter_CELT();

  // convert to PCM
  int convertToPCM(char *buffer, int size);
};

} // end of namespace qtbrynhildr

#endif // CONVERTER_CELT_H
