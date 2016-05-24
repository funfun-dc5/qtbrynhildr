// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// mousebuffer.cpp

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "mousebuffer.h"

namespace qtbrynhildr {

// constructor
MouseBuffer::MouseBuffer(int size)
  :
  topPos(0),
  nextPos(0),
  enabled(true),
  // for DEBUG
  outputLog(false)
{
  // allocate buffer
  buffer = new MouseInfo[size];
  bufferSize = size;
}

// destructor
MouseBuffer::~MouseBuffer()
{
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }
}

// put data to ring buffer
int MouseBuffer::put(MouseInfoType type, MouseInfoValue value)
{
  if (!enabled){ // NOT enabled
	return 0;
  }
  if (size() == bufferSize){ // Full
	return -1;
  }
  // for DEBUG
  if (outputLog){
	switch(type){
	case TYPE_MOUSE_RIGHT_BUTTON:
	  cout << "[MouseBuffer] : RIGHT_BUTTON : value = " << (int)value.button << " : size() = " << size() << endl << flush;
	  break;
	case TYPE_MOUSE_LEFT_BUTTON:
	  cout << "[MouseBuffer] : LEFT_BUTTON  : value = " << (int)value.button << " : size() = " << size() << endl << flush;
	  break;
	case TYPE_MOUSE_WHEEL:
	  cout << "[MouseBuffer] : WHEEL        : value = " << (int)value.wheel << " : size() = " << size() <<  endl << flush;
	  break;
	default:
	  ABORT();
	  break;
	}
  }

  // set informations
  buffer[nextPos].type = type;
  buffer[nextPos].value = value;
  nextPos++;
  if (nextPos == bufferSize){
	nextPos = 0;
  }

  return 1;
}

// get data from ring buffer
MouseInfo *MouseBuffer::get()
{
  MouseInfo *ret = 0;
  if (enabled && size() != 0){
	ret = &buffer[topPos];
	topPos++;
	if (topPos == bufferSize){
	  topPos = 0;
	}
  }
  return ret;
}

// clear buffer
void MouseBuffer::clear()
{
  // reset
  topPos = 0;
  nextPos = 0;
}

// get available data size
int MouseBuffer::size() const
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

// set mouse position
void MouseBuffer::setMousePos(MOUSE_POS curPos)
{
  pos = curPos;
}

// get mouse position
MOUSE_POS MouseBuffer::getMousePos() const
{
  return pos;
}

} // end of namespace qtbrynhildr
