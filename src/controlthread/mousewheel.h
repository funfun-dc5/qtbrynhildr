// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef MOUSEWHEEL_H
#define MOUSEWHEEL_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

class MouseWheel
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // buffer
  MOUSE_WHEEL *buffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  MouseWheel(int size);
  // destructor
  virtual ~MouseWheel();

  // clear buffer
  void clear();

  // get available data size
  int size() const;

  // put wheel to ring buffer
  int putWheel(MOUSE_WHEEL wheel);

  // get wheel from ring buffer
  MOUSE_WHEEL getWheel();
};

} // end of namespace qtbrynhildr

#endif // MOUSEWHEEL_H
