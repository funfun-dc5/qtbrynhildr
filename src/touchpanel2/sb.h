// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SB_H
#define SB_H

// System Header

// Qt Header
#include <QEvent>

// Local Header
#include "control/mousebuffer.h"
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
  bool event(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

#if QTB_NEW_DESKTOPWINDOW
  // mouse event
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
#endif // QTB_NEW_DESKTOPWINDOW

private:
  // mouse buffer
  MouseBuffer *mouseBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // previous click button
  MouseInfoType previousClick;

  // previous click time
  QDateTime previousClickTime;

#if QTB_NEW_DESKTOPWINDOW
  // graphics view
  GraphicsView *graphicsView;
#endif // QTB_NEW_DESKTOPWINDOW

  // output log flag
  bool outputLog;

signals:
  // refresh menu
  void refreshMenu();
};

} // end of namespace qtbrynhildr

#endif // SB_H
