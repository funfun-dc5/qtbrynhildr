// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "framecontroller.h"

namespace qtbrynhildr {

// constructor
FrameController::FrameController()
  :previousTime(0)
  ,startTimeOfDecode(0)
  ,endTimeOfDecode(0)
  ,decodeTime(0)
  // for DEBUG
  ,outputLog(false)
{
  reset();
}

// destructor
FrameController::~FrameController()
{
}

// adjust frame
bool FrameController::adjust(char frame_no, unsigned long frameInterval)
{
  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  qint64 diffTime = 0;
  if (previousTime != 0){
	diffTime = currentTime - previousTime;
  }
  previousTime = currentTime;

  if (outputLog){
	cout << "== frame_no: " << (int)frame_no << endl;
	cout << "diff time  : " << diffTime << " (ms)" << endl;
	cout << "decode time: " << decodeTime << " (ms)" << endl;
	cout << "FI         : " << frameInterval << " (ms)" << endl << flush;
  }

#if 0 // for TEST
  if (diffTime > frameInterval * 2){
	if (outputLog){
	  cout << "drop a frame!" << endl << flush;
	}
	return false;
  }
  else {
	return true;
  }
#else // 0 // for TEST
  return true; // default
#endif // 0 // for TEST
}

} // end of namespace qtbrynhildr
