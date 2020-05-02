// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Local Header
#include "soundbuffer.h"

namespace qtbrynhildr {

// constructor
SoundBuffer::SoundBuffer(int size)
  :RingBuffer(size)
{
}

// destructor
SoundBuffer::~SoundBuffer()
{
}

} // end of namespace qtbrynhildr
