// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef MOUSEBUFFER_H
#define MOUSEBUFFER_H
// Local Header
#include "common/protocols.h"
#include "mousebutton.h"
#include "mousewheel.h"

namespace qtbrynhildr {

// mouse position (NOTE: class)
typedef struct {
  POS x;
  POS y;
} MOUSE_POS;

// MouseBuffer
class MouseBuffer
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
public:
  // type of mouse information
  typedef enum {
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_BACK,
	MOUSE_BUTTON_FORWARD,
	MOUSE_BUTTON_FILEDROP,
	MOUSE_BUTTON_MOVE_TOPSIDE,
	MOUSE_BUTTON_MOVE_BOTTOMSIDE,
	MOUSE_BUTTON_MOVE_LEFTSIDE,
	MOUSE_BUTTON_MOVE_RIGHTSIDE,
	MOUSE_BUTTON_NUM,
	MOUSE_BUTTON_INVALID = MOUSE_BUTTON_NUM
  } MOUSE_BUTTON_ID;

private:
  // button queues
  MouseButton *buttons[MOUSE_BUTTON_NUM];

  // wheel queue
  MouseWheel *wheel;

  // mouse position
  MOUSE_POS pos;

  // enabled flag
  bool enabled;

  // output log flag
  const bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  MouseBuffer(int size);
  // destructor
  ~MouseBuffer();

  // clear buffer
  void clear();

  // set enabled flag
  void setEnabled(bool enabled)
  {
	this->enabled = enabled;
  }

  // for button queue
  int putButton(MOUSE_BUTTON_ID button, MOUSE_BUTTON value);
  MOUSE_BUTTON getButton(MOUSE_BUTTON_ID button);

  // for wheel queue
  int putWheel(MOUSE_WHEEL value);
  MOUSE_WHEEL getWheel();

  // set mouse position
  void setPos(MOUSE_POS pos);

  // set mouse posittion
  void setPos(POS x, POS y);

  // get mouse position
  MOUSE_POS getPos() const;
};

} // end of namespace qtbrynhildr

#endif // MOUSEBUFFER_H
