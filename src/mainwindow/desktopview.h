// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QEvent>
#include <QMouseEvent>
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
  // scaling factor
  qreal scalingFactor;

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

  // scale
  void setScale(qreal scalingFactor);

protected:
  // paint event
  void paintEvent(QPaintEvent *event);

  // viewport event for event handling (touchpanel)
  bool viewportEvent(QEvent *event);

  // -----------------------------------------------------------------------------------
  // KeroRemote 1 Finger Operation
  // -----------------------------------------------------------------------------------
  bool oneFingerEventForKeroRemote(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // QtBrynhildr 1 Finger Operation
  // -----------------------------------------------------------------------------------
  bool oneFingerEventForQtBrynhildr(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // KeroRemote 2 Finger Operation
  // -----------------------------------------------------------------------------------
  bool twoFingerEventForKeroRemote(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // QtBrynhildr 2 Finger Operation
  // -----------------------------------------------------------------------------------
  bool twoFingerEventForQtBrynhildr(QTouchEvent *touchEvent);

  // -----------------------------------------------------------------------------------
  // 3 Finger Operation
  // -----------------------------------------------------------------------------------
  bool threeFingerEvent(QTouchEvent *touchEvent);

private:
  // get scaling factor for full screen
  inline qreal getScalingFactorForFullScreen() const
  {
	return settings->getDesktopScalingFactorLimit();
  }

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPVIEW_H
