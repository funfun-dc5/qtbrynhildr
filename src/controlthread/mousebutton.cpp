// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "mousebutton.h"

namespace qtbrynhildr {

// constructor
MouseButton::MouseButton(int size)
  :buffer(0)
  ,bufferSize(0)
  ,topPos(0)
  ,nextPos(0)
  ,outputLog(false)
{
  Q_UNUSED(outputLog);

  // allocate buffer
  buffer = new MOUSE_BUTTON[size];
  bufferSize = size;
}

// destructor
MouseButton::~MouseButton()
{
  // delete objects
  // buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }
}

// clear buffer
void MouseButton::clear()
{
  // reset
  topPos = 0;
  nextPos = 0;
}

// get available data size
int MouseButton::size() const
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

// put button to ring buffer
int MouseButton::putButton(MOUSE_BUTTON button)
{
  if (size() == bufferSize){ // Full
	return -1;
  }

  // set information
  buffer[nextPos] = button;
  nextPos++;
  if (nextPos == bufferSize){
	nextPos = 0;
  }

  return 0;
}

// get button from ring buffer
MOUSE_BUTTON MouseButton::getButton()
{
  MOUSE_BUTTON ret = 0;
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
