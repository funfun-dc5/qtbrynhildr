// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QEvent>
#include <QPaintEvent>
#include <QScrollArea>
#include <QWidget>

// Local Header
#include "controlthread/keybuffer.h"
#include "mainwindow/desktoppanel.h"
#include "qtbrynhildr.h"
#include "settings.h"


namespace qtbrynhildr {

//class QtBrynhildr;

class DesktopView : public QScrollArea, public DesktopPanel
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Type
  //-------------------------------------------------------------------------------
private:
  // Touch Operation Type
  typedef enum {
	TOP_TYPE_UNKNOWN,
	TOP_TYPE_1POINT,
	TOP_TYPE_2POINT,
	TOP_TYPE_3POINT
  } TOP_TYPE;

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // flags
  TOP_TYPE topType;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopView(QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  virtual ~DesktopView();

protected:
  // paint event
  void paintEvent(QPaintEvent *event);

  // viewport event for event handling (touchpanel)
  bool viewportEvent(QEvent *event);

  // -----------------------------------------------------------------------------------
  // KeroRemote Compatible Operation (1 finger)
  // -----------------------------------------------------------------------------------
  bool oneFingerEventForKeroRemote(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // QtBrynhildr Operation (1 finger)
  // -----------------------------------------------------------------------------------
  bool oneFingerEventForQtBrynhildr(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // 2 Finger Operation
  // -----------------------------------------------------------------------------------
  bool twoFingerEvent(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // 3 Finger Operation
  // -----------------------------------------------------------------------------------
  bool threeFingerEvent(QTouchEvent *touchEvent);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPVIEW_H
