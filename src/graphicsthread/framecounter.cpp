// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDateTime>

// Local Header
#include "framecounter.h"

namespace qtbrynhildr {

// constructor
FrameCounter::FrameCounter()
  :
  frameCounter(0)
  ,previousGetFrameRateTime(0)
  ,totalFrameCounter(0)
  // for DEBUG
  ,outputLog(true)
{
}

// destructor
FrameCounter::~FrameCounter()
{
}

// reset frame counter
void FrameCounter::reset()
{
  // reset frame counter
  frameCounter = 0;

  // reset previous frame time
  previousGetFrameRateTime = 0;
}

// count up frame counter
void FrameCounter::countUp()
{
  frameCounter++;
}

// get frame rate
double FrameCounter::getFrameRate()
{
  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  double fps = 0.0;

  if (previousGetFrameRateTime != 0){
	qint64 diffMSeconds = currentTime - previousGetFrameRateTime;
	if (diffMSeconds != 0){
	  fps = frameCounter / ((double)diffMSeconds)*1000;
	  //cout << "frameCounter = " << frameCounter << endl;
	  //cout << "diffMSeconds = " << diffMSeconds << endl << flush;
	}
  }
  previousGetFrameRateTime = currentTime;
  totalFrameCounter += frameCounter;
  frameCounter = 0;

  return fps;
}

} // end of namespace qtbrynhildr
