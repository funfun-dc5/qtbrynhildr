// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_keybord_jp.h

#ifndef SOFTWARE_KEYBOARD_JP_H
#define SOFTWARE_KEYBOARD_JP_H
// Qt Header

// Local Header
#include "software_keyboard.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

// SoftwareKeyboard_JP
class SoftwareKeyboard_JP : public SoftwareKeyboard
{
public:
  // constructor
  SoftwareKeyboard_JP(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent = 0);

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

#endif // SOFTWARE_KEYBOARD_JP_H
