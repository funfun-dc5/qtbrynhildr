// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "mousewheel.h"

namespace qtbrynhildr {

// constructor
MouseWheel::MouseWheel(int size)
  :buffer(0)
  ,bufferSize(0)
  ,topPos(0)
  ,nextPos(0)
  ,outputLog(true)
{
  Q_UNUSED(outputLog);

  // allocate buffer
  buffer = new MOUSE_WHEEL[size];
  bufferSize = size;
}

// destructor
MouseWheel::~MouseWheel()
{
  // delete objects
  // buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
	bufferSize = 0;
  }
}

// clear buffer
void MouseWheel::clear()
{
  // reset
  topPos = 0;
  nextPos = 0;
}

// get available data size
int MouseWheel::size() const
{
  int len = 0;

  if (topPos < nextPos){
	// ----------------------------
	// |   |----------------|     |
	// ----------------------------
	//    topPos            nextPos
	len = nextPos - topPos;
  }
  else if (topPos > nextPos){
	// ----------------------------
	// |----|               |-----|
	// ----------------------------
	//       nextPos         topPos
	len = bufferSize - topPos + nextPos;
  }

  return len;
}

// put wheel to ring buffer
int MouseWheel::putWheel(MOUSE_WHEEL wheel)
{
  if (size() == bufferSize){ // Full
	return -1;
  }

  // set information
  buffer[nextPos] = wheel;
  nextPos++;
  if (nextPos == bufferSize){
	nextPos = 0;
  }

  return 0;
}

// get wheel from ring buffer
MOUSE_WHEEL MouseWheel::getWheel()
{
  MOUSE_WHEEL ret = 0;
  if (size() != 0){
	ret = buffer[topPos];
	topPos++;
	if (topPos == bufferSize){
	  topPos = 0;
	}
  }
  return ret;
}

} // end of namespace qtbrynhildr
