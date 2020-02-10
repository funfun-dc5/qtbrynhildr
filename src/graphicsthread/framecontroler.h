// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef FRAMECONTROLER_H
#define FRAMECONTROLER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


namespace qtbrynhildr {

class FrameControler
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  FrameControler();
  // destructor
  virtual ~FrameControler();

  // adjust frame
  bool adjust();

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // FRAMECONTROLER_H
