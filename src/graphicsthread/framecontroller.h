// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef FRAMECONTROLLER_H
#define FRAMECONTROLLER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


namespace qtbrynhildr {

class FrameController
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
  FrameController();
  // destructor
  virtual ~FrameController();

  // adjust frame
  bool adjust(int frame_no);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // FRAMECONTROLLER_H
