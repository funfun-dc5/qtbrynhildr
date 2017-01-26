// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SK_H
#define SK_H

// System Header

// Qt Header

// Local Header
#include "control/keybuffer.h"
#include "settings.h"
#include "software_keyboard.h"

namespace qtbrynhildr {

// SoftwareKeyboard
class SK : public SoftwareKeyboard
{
public:
  // constructor
  SK(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent = 0);
  // destructor
  //  ~SK();

protected:
  // key down
  void keyDown(uchar key);
  
  // key up
  void keyUp(uchar key);

private:
  // settings
  Settings *settings;

  // key buffer
  KeyBuffer *keyBuffer;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SK_H
