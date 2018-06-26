// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef CONVERTER_H
#define CONVERTER_H

// Qt Header

// Local Header

namespace qtbrynhildr {

// Converter
class Converter
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
protected:
  // sample rate
  int samplerate;

  // channels
  int channels;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  Converter(int samplerate, int channels);
  // destructor
  virtual ~Converter();

  // convert to PCM
  virtual int convertToPCM(char *buffer, int len) = 0;
};

} // end of namespace qtbrynhildr

#endif // CONVERTER_H
