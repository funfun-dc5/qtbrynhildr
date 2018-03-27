// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header

// Local Header
#include "keybuffer.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

// constructor
KeyBuffer::KeyBuffer(int size)
  :topPos(0)
  ,nextPos(0)
  ,enabled(true)
  // for DEBUG
  ,outputLog(false)
{
  // allocate buffer
  buffer = new KeyInfo[size];
  bufferSize = size;
}

// destructor
KeyBuffer::~KeyBuffer()
{
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }
}

// put data to ring buffer
int KeyBuffer::put(uchar keycode, KEYCODE_FLG keycode_flg)
{
  if (!enabled){ // NOT enabled
	return 0;
  }
  if (size() == bufferSize){ // Full
	return -1;
  }

  // set informations
  buffer[nextPos].keycode = keycode;
  buffer[nextPos].keycode_flg = keycode_flg;
  nextPos++;
  if (nextPos == bufferSize){
	nextPos = 0;
  }

  // for DEBUG
  if (outputLog){
	ios::fmtflags flags = cout.flags();
	switch(keycode_flg){
	case KEYCODE_FLG_KEYUP:
	  cout << "[KeyBuffer] : KEYUP   : VK_Code = " << hex << (int)keycode << ": size() = " << dec << size() << endl << flush;
	  break;
	case KEYCODE_FLG_KEYDOWN:
	  cout << "[KeyBuffer] : KEYDOWN : VK_Code = " << hex << (int)keycode << ": size() = " << dec << size() <<endl << flush;
	  break;
	default:
	  // unknown KEYCODE_FLG
	  ABORT();
	  break;
	}
	cout.flags(flags);
  }

  return 1;
}

// get data from ring buffer
KeyInfo *KeyBuffer::get()
{
  KeyInfo *ret = 0;
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
void KeyBuffer::clear()
{
  // reset
  topPos = 0;
  nextPos = 0;
}

// get available data size
int KeyBuffer::size() const
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

} // end of namespace qtbrynhildr
