// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYEVENT_H
#define KEYEVENT_H
// Common Header

// System Header

// Qt Header

// Local Header

#ifdef __cplusplus
public:
#endif // __cplusplus

// shift key control
typedef enum {
  SHIFTKEY_THROUGH,
  SHIFTKEY_NEED,
  SHIFTKEY_NONEED,
} ShiftKeyControl;

#ifdef __cplusplus
private:
using Key = Qt::Key;
#else // __cplusplus
typedef unsigned char uchar;
#endif // __cplusplus

// key table for event convert
typedef struct {
  Key key;
  uchar VK_Code;
  ShiftKeyControl shiftKeyControl;
} KeyEvent;

#endif // KEYEVENT_H
