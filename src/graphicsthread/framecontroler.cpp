// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QDateTime>

// Local Header
#include "framecontroler.h"

namespace qtbrynhildr {

// constructor
FrameControler::FrameControler()
  :
  // for DEBUG
  outputLog(false)
{
}

// destructor
FrameControler::~FrameControler()
{
}

// adjust frame
bool FrameControler::adjust(int frame_no)
{
  if (outputLog){
	cout << "frame_no : " << frame_no << endl << flush;
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	cout << "time     : " << currentTime << endl << flush;
  }
  return true;
}

} // end of namespace qtbrynhildr
