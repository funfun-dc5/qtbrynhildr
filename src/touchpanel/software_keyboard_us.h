// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOFTWARE_KEYBOARD_US_H
#define SOFTWARE_KEYBOARD_US_H
// Qt Header

// Local Header
#include "software_keyboard.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

// SoftwareKeyboard_US
class SoftwareKeyboard_US : public SoftwareKeyboard
{
public:
  // constructor
  SoftwareKeyboard_US(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent = 0);

protected:
  // pressed key
  void pressedKey(ID_KEY id);

  // released key
  void releasedKey(ID_KEY id);

private:
  // initialize key top text
  void initializeKeyTopText();

  // shift key
  void pressedShiftKey();

  // Fn key
  void pressedFnKey();
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_US_H
