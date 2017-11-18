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
  SK(KeyBuffer *keyBuffer, QtBrynhildr *qtbrynhildr);
  // destructor
  //  ~SK();

protected:
  // pressed key
  void pressedKey(ID_KEY id) override;

  // released key
  void releasedKey(ID_KEY id) override;

  // key down
  void keyDown(uchar key) override;
  
  // key up
  void keyUp(uchar key) override;

private:
  // key buffer
  KeyBuffer *keyBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SK_H
