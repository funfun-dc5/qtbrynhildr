// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDateTime>

// Local Header


namespace qtbrynhildr {

class FrameCounter
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // frame counter
  unsigned int frameCounter;

  // previous get frame rate time
  qint64 previousGetFrameRateTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  FrameCounter();
  // destructor
  virtual ~FrameCounter();

  // reset frame counter
  void reset();

  // count up frame counter
  void countUp();

  // get frame counter
  unsigned int getFrameCounter()
  {
	return frameCounter;
  }

  // get frame rate
  double getFrameRate();

  // get total frame counter
  unsigned int getTotalFrameCounter() const
  {
	return totalFrameCounter;
  }

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // FRAMECOUNTER_H
