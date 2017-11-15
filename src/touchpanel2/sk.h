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

class QtBrynhildr;

// SoftwareKeyboard
class SK : public SoftwareKeyboard
{
public:
  // constructor
  SK(Settings *settings, KeyBuffer *keyBuffer, QtBrynhildr *qtbrynhildr);
  // destructor
  //  ~SK();

protected:
  // pressed key
  void pressedKey(ID_KEY id);

  // released key
  void releasedKey(ID_KEY id);

  // key down
  void keyDown(uchar key) override;
  
  // key up
  void keyUp(uchar key) override;

private:
  // settings
  Settings *settings;

  // key buffer
  KeyBuffer *keyBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SK_H
