// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// converter.cpp

// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "converter.h"

namespace qtbrynhildr {

// constructor
Converter::Converter(int samplerate, int channels)
  :
  samplerate(samplerate),
  channels(channels),
  outputLog(false)
{
}

// destructor
Converter::~Converter()
{
}

} // end of namespace qtbrynhildr
