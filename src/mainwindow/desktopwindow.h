// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPWINDOW_H
#define DESKTOPWINDOW_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QWidget>

// Local Header
#include "mainwindow/desktoppanel.h"

namespace qtbrynhildr {

class QtBrynhildr;

// DesktopWindow
#if defined(Q_OS_WIN)
class DesktopWindow : public QWidget, public DesktopPanel, public QAbstractNativeEventFilter
#else // defined(Q_OS_WIN)
class DesktopWindow : public QWidget, public DesktopPanel
#endif // defined(Q_OS_WIN)
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // first pos
  QPoint firstPos;

  // last pos
  QPoint lastPos;

  // current pos
  QPoint currentPos;

  // on draging
  bool onDraging;

  // on area mode
  bool onAreaMode;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopWindow(QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  ~DesktopWindow();

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

  // widget enter event
  void enterEvent(QEvent *event);

  // widget leave event
  void leaveEvent(QEvent *event);

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

#if QTB_DRAG_AND_DROP_SUPPORT
  // drag and drop
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if QTB_MOUSE_TRACKING_FOCUS_MODE
  // focus in and out
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
#endif // QTB_MOUSE_TRACKING_FOCUS_MODE

private:
#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)

  // enter area mode
  void enterAreaMode(QRect rect);
public:
  // leave area mode
  void leaveAreaMode();
};

} // end of namespace qtbrynhildr

#endif // DESKTOPWINDOW_H
