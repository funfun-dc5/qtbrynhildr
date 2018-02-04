// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef MOUSEBUFFER_H
#define MOUSEBUFFER_H
// Local Header
#include "common/protocols.h"

namespace qtbrynhildr {

// type of mouse information
typedef enum {
  TYPE_MOUSE_INVALID,
  TYPE_MOUSE_RIGHT_BUTTON,
  TYPE_MOUSE_LEFT_BUTTON,
  TYPE_MOUSE_MIDDLE_BUTTON,
  TYPE_MOUSE_BACK_BUTTON,
  TYPE_MOUSE_FORWARD_BUTTON,
  TYPE_MOUSE_WHEEL,
  TYPE_MOUSE_FILEDROP
} MouseInfoType;

// value of mouse information
typedef union {
  MOUSE_BUTTON button;
  MOUSE_WHEEL wheel;
} MouseInfoValue;

// mouse information
struct MouseInfo {
  MouseInfoType type;
  MouseInfoValue value;
};

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
private:
  // buffer
  MouseInfo *buffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

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

  // put data to ring buffer
  int put(MouseInfoType type, MouseInfoValue value);

  // get data from ring buffer
  MouseInfo *get();

  // clear buffer
  void clear();

  // get available data size
  int size() const;

  // set enabled flag
  void setEnabled(bool enabled)
  {
	this->enabled = enabled;
  }

  // set mouse position
  void setMousePos(MOUSE_POS curPos);

  // get mouse position
  MOUSE_POS getMousePos() const;
};

} // end of namespace qtbrynhildr

#endif // MOUSEBUFFER_H
