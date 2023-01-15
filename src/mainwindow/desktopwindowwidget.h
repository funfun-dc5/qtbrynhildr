// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPWINDOWWIDGET_H
#define DESKTOPWINDOWWIDGET_H
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
#include "mainwindow/desktopframe.h"

namespace qtbrynhildr {

class QtBrynhildr;

// DesktopWindowWidget
#if defined(Q_OS_WIN)
class DesktopWindow : public QWidget, public DesktopPanel, public QAbstractNativeEventFilter
#else // !defined(Q_OS_WIN)
class DesktopWindow : public QWidget, public DesktopPanel
#endif // !defined(Q_OS_WIN)
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
  DesktopWindow(QtBrynhildr *qtbrynhildr, QWidget *parent = nullptr);
  // destructor
  ~DesktopWindowWidget();

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
#if QT_VERSION < 0x060000
  void enterEvent(QEvent *event);
#else // QT_VERSION >= 0x060000
  void enterEvent(QEnterEvent *event);
#endif // QT_VERSION >= 0x060000

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
#if QT_VERSION < 0x060000
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#else // QT_VERSION >= 0x060000
  bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);
#endif // QT_VERSION >= 0x060000
#endif // defined(Q_OS_WIN)

  // enter area mode
  void enterAreaMode(QRect rect);
public:
  // leave area mode
  void leaveAreaMode();
};

} // end of namespace qtbrynhildr

#endif // DESKTOPWINDOWWIDGET_H
