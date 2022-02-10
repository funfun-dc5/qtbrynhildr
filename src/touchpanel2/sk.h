// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef SK_H
#define SK_H

// System Header

// Qt Header
#include <QEvent>

// Local Header
#include "controlthread/keybuffer.h"
#include "settings.h"
#include "software_keyboard.h"
#if defined(QTB_DEV_TOUCHPANEL)
#include "mainwindow/graphicsview.h"
#endif // defined(QTB_DEV_TOUCHPANEL)

namespace qtbrynhildr {

class QtBrynhildr;

// SoftwareKeyboard
class SK : public SoftwareKeyboard
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // key buffer
  KeyBuffer *keyBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

#if defined(QTB_DEV_TOUCHPANEL)
  // graphics view
  GraphicsView *graphicsView;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  SK(KeyBuffer *keyBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent);
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

  // for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
  bool event(QEvent *event) override;
#endif // defined(QTB_DEV_TOUCHPANEL)

#if defined(QTB_DEV_TOUCHPANEL)
  // mouse event
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
#endif // defined(QTB_DEV_TOUCHPANEL)
};

} // end of namespace qtbrynhildr

#endif // SK_H
