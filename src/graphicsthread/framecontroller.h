// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef FRAMECONTROLLER_H
#define FRAMECONTROLLER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDateTime>

// Local Header


namespace qtbrynhildr {

class FrameController
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // previous time of adjust()
  qint64 previousTime;

  // decode start time
  qint64 startTimeOfDecode;

  // decode end time
  qint64 endTimeOfDecode;

  // decode time
  qint64 decodeTime;

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

  // reset controller
  void reset()
  {
	previousTime = 0;
	startTimeOfDecode = 0;
	endTimeOfDecode = 0;
	decodeTime = 0;
  }

  // decode start time
  void startDecode()
  {
	startTimeOfDecode = QDateTime::currentDateTime().toMSecsSinceEpoch();
  }

  // decode end time
  void endDecode()
  {
	endTimeOfDecode = QDateTime::currentDateTime().toMSecsSinceEpoch();
	decodeTime = endTimeOfDecode - startTimeOfDecode;
  }

  // adjust frame
  bool adjust(char frame_no, unsigned long frameInterval);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // FRAMECONTROLLER_H
