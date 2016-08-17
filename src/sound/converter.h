// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// converter.h

#ifndef CONVERTER_H
#define CONVERTER_H

// Qt Header

// Local Header

namespace qtbrynhildr {

// Converter
class Converter
{
public:
  // constructor
  Converter(int samplerate, int channels);
  // destructor
  virtual ~Converter();

  // convert to PCM
  virtual int convertToPCM(char *buffer, int len) = 0;

protected:
  // sample rate
  int samplerate;

  // channels
  int channels;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // CONVERTER_H
