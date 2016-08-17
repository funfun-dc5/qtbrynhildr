// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// converter_celt.h

#ifndef CONVERTER_CELT_H
#define CONVERTER_CELT_H

// Qt Header

// CELT
#include "celt.h"

// Local Header
#include "converter.h"

namespace qtbrynhildr {

// Converter_CELT
class Converter_CELT : public Converter
{
public:
  // constructor
  Converter_CELT(int samplerate, int channels);
  // destructor
  ~Converter_CELT();

  // convert to PCM
  int convertToPCM(char *buffer, int size);

private:
  // frame size
  int frameSize;

  // mode
  CELTMode *mode;

  // decorder
  CELTDecoder *decoder;

  // work buffer
  char *workBuffer;
};

} // end of namespace qtbrynhildr

#endif // CONVERTER_CELT_H
