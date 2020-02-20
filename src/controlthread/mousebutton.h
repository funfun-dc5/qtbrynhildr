// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef MOUSEBUTTON_H
#define MOUSEBUTTON_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

class MouseButton
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // buffer
  MOUSE_BUTTON *buffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

  // output log flag
  //bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  MouseButton(int size);
  // destructor
  virtual ~MouseButton();

  // clear buffer
  void clear();

  // get available data size
  int size() const;

  // put button to ring buffer
  int putButton(MOUSE_BUTTON button);

  // get button from ring buffer
  MOUSE_BUTTON getButton();
};

} // end of namespace qtbrynhildr

#endif // MOUSEBUTTON_H
