// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SB_H
#define SB_H

// System Header

// Qt Header
#include <QEvent>
#include <QShowEvent>

// Local Header
#include "controlthread/mousebuffer.h"
#include "settings.h"
#include "software_button.h"
#if QTB_NEW_DESKTOPWINDOW
#include "mainwindow/graphicsview.h"
#endif // QTB_NEW_DESKTOPWINDOW

namespace qtbrynhildr {

class QtBrynhildr;

// SoftwareButton
class SB : public SoftwareButton
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // mouse buffer
  MouseBuffer *mouseBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // previous click button
  MouseBuffer::MOUSE_BUTTON_ID previousClickButton;

  // previous click time
  QDateTime previousClickTime;

  // mouse left button
  bool pressedMouseLeftButton;

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
  SB(MouseBuffer *mouseBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent);
  // destructor
  //  ~SB();

protected:
  // pressed button
  void pressedButton(ID_BUTTON id) override;

  // released button
  void releasedButton(ID_BUTTON id) override;

  // for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
  virtual bool event(QEvent *event) override;
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_NEW_DESKTOPWINDOW
  // mouse event
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
#endif // QTB_NEW_DESKTOPWINDOW

  // window show event
  virtual void showEvent(QShowEvent *event) override;

#if defined(QTB_DEV_TOUCHPANEL)
  // hide event
  virtual void hideEvent(QHideEvent *event) override;
#endif // defined(QTB_DEV_TOUCHPANEL)

signals:
  // refresh menu
  void refreshMenu();
};

} // end of namespace qtbrynhildr

#endif // SB_H
