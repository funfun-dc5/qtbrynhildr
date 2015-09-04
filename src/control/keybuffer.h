// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// keybuffer.h

#ifndef KEYBUFFER_H
#define KEYBUFFER_H
// Local Header
#include "protocols.h"

namespace qtbrynhildr {

// key information
struct KeyInfo {
  char			keycode;		// keycode (VK_*)
  KEYCODE_FLG	keycode_flg;	// KEYCODE_FLG_KEYUP/KEYDOWN
};

// KeyBuffer
class KeyBuffer
{
public:
  // constructor
  KeyBuffer(int size);
  // destructor
  ~KeyBuffer();

  // put data to ring buffer
  int put(char keycode, KEYCODE_FLG keycode_flg);

  // get data from ring buffer
  KeyInfo *get();

  // clear buffer
  void clear();

  // get available data size
  int size() const;

private:
  // buffer
  KeyInfo *buffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // KEYBUFFER_H
