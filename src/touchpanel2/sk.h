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
#if QTB_NEW_DESKTOPWINDOW
#include "mainwindow/desktoppanel.h"
#include "mainwindow/graphicsview.h"
#endif // QTB_NEW_DESKTOPWINDOW

namespace qtbrynhildr {

class QtBrynhildr;

// SoftwareKeyboard
class SK : public SoftwareKeyboard
{
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

#if QTB_NEW_DESKTOPWINDOW
  // mouse event
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
#endif // QTB_NEW_DESKTOPWINDOW

private:
  // key buffer
  KeyBuffer *keyBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

#if QTB_NEW_DESKTOPWINDOW
  // desktop panel
  DesktopPanel *desktopPanel;

  // graphics view
  GraphicsView *graphicsView;
#endif // QTB_NEW_DESKTOPWINDOW

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SK_H
