// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef SK_H
#define SK_H

// System Header

// Qt Header
#include <QEvent>

// Local Header
#include "controlthread/keybuffer.h"
#include "settings.h"
#include "software_keyboard.h"
#if QTB_NEW_DESKTOPWINDOW
#include "mainwindow/graphicsview.h"
#endif // QTB_NEW_DESKTOPWINDOW

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

#if QTB_NEW_DESKTOPWINDOW
  // graphics view
  GraphicsView *graphicsView;
#endif // QTB_NEW_DESKTOPWINDOW

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
  bool event(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_NEW_DESKTOPWINDOW
  // mouse event
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
#endif // QTB_NEW_DESKTOPWINDOW
};

} // end of namespace qtbrynhildr

#endif // SK_H
