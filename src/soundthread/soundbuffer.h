// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// SoundBuffer
class SoundBuffer : public RingBuffer
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
  SoundBuffer(int size);
  // destructor
  ~SoundBuffer();

private:

};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
