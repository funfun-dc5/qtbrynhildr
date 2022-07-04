// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2022- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPPANELWIDGET_H
#define DESKTOPPANELWIDGET_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QSize>
#include <QWidget>

// Local Header
#include "mainwindow/desktopframe.h"

namespace qtbrynhildr {

class QtBrynhildr;

// DesktopPanelWidget
class DesktopPanelWidget : public QWidget, public DesktopFrame
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopPanelWidget(QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  ~DesktopPanelWidget();

  // resize desktop
  void resizeDesktop(int width, int height);

  // refresh desktop
  void updateDesktop();

  // minimum size hint
  QSize minimumSizeHint() const;

  // size hint
  QSize sizeHint() const;

protected:
  // for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // event
  bool event(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // window event
  void paintEvent(QPaintEvent *event);

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPPANELWIDGET_H
