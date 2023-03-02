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
#include <QPoint>
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
  // current pos
  QPoint currentPos;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopPanelWidget(QtBrynhildr *qtbrynhildr, QWidget *parent);
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

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  // get current pos
  QPoint getCurrentPos()
  {
	return currentPos;
  }

  // set current pos
  void setCurrentPos(QPoint point)
  {
	currentPos = point;
  }

protected:
  // for event handling
  // event
  bool event(QEvent *event);

  // window event
  void paintEvent(QPaintEvent *event);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPPANELWIDGET_H
