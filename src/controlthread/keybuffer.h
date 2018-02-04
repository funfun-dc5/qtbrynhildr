// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYBUFFER_H
#define KEYBUFFER_H
// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

// key information
struct KeyInfo {
  uchar			keycode;		// keycode (VK_*)
  KEYCODE_FLG	keycode_flg;	// KEYCODE_FLG_KEYUP/KEYDOWN
};

// KeyBuffer
class KeyBuffer
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // buffer
  KeyInfo *buffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

  // enabled flag
  bool enabled;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  KeyBuffer(int size);
  // destructor
  ~KeyBuffer();

  // put data to ring buffer
  int put(uchar keycode, KEYCODE_FLG keycode_flg);

  // get data from ring buffer
  KeyInfo *get();

  // clear buffer
  void clear();

  // get available data size
  int size() const;

  // set enabled flag
  void setEnabled(bool enabled)
  {
	this->enabled = enabled;
  }

};

} // end of namespace qtbrynhildr

#endif // KEYBUFFER_H
