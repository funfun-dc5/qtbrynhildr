// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPPANEL_H
#define DESKTOPPANEL_H
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
#include "mainwindow/desktoppanelwidget.h"
#include "qtbrynhildr.h"
#include "settings.h"


namespace qtbrynhildr {

class QtBrynhildr;

class DesktopPanel : public QScrollArea
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
  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // desktop panel widget
  DesktopPanelWidget *desktopPanelWidget;

  // key buffer
  KeyBuffer *keyBuffer;

  // Touch Operation Type
  TOP_TYPE topType;

  // for software button
  QRect softwareButtonRect;

  // for software keyboard
  QRect softwareKeyboardRect;

  // scaling factor
  qreal scalingFactor;

  // scaling factor for fullscreen
  qreal scalingFactorForFullScreen;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopPanel(QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  virtual ~DesktopPanel();

  // scale
  void setScale(qreal scalingFactor);

  // resize desktop
  void resizeDesktop(int width, int height);

  // update desktop
  void updateDesktop();

  // mouse press event software panel
  void mousePressEventForSP(QMouseEvent *event);
  // mouse release event software panel
  void mouseReleaseEventForSP(QMouseEvent *event);
  // mouse move event software panel
  void mouseMoveEventForSP(QMouseEvent *event);
  // mouse move
  void mouseMove(QPoint mousePos, bool marker = true);
  // mouse move relatively
  void mouseMoveRelatively(QPoint mousePos, bool marker = true);

  // for software button
  void setSoftwareButtonRect(QRect rect)
  {
	softwareButtonRect = rect;
  }

  // for software keyboard
  void setSoftwareKeyboardRect(QRect rect)
  {
	softwareKeyboardRect = rect;
  }

  // size hint
  QSize sizeHint() const;

protected:
  // viewport size hint
  QSize viewportSizeHint() const;

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

#endif // DESKTOPPANEL_H
