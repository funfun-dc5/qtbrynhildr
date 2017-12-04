// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMargins>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QSize>
#include <QRect>

// Local Header
#include "control/keybuffer.h"
#include "mainwindow/desktoppanel.h"
#include "settings.h"

namespace qtbrynhildr {

class QtBrynhildr;

#if defined(Q_OS_WIN)
class GraphicsView : public QGraphicsView, public QAbstractNativeEventFilter
#else // defined(Q_OS_WIN)
class GraphicsView : public QGraphicsView
#endif // defined(Q_OS_WIN)
{
  Q_OBJECT

public:
  // constructor
  GraphicsView(QGraphicsScene *scene, QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  ~GraphicsView();

  // size hint
  QSize sizeHint() const;

  // scale
  void setScale(qreal scalingFactor);

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // mouse press event software panel
  void mousePressEventForSP(QMouseEvent *event);
  // mouse release event software panel
  void mouseReleaseEventForSP(QMouseEvent *event);
  // mouse move event software panel
  void mouseMoveEventForSP(QMouseEvent *event);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

#if defined(QTB_DEV_TOUCHPANEL)
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
#endif // defined(QTB_DEV_TOUCHPANEL)

protected:
  // for event handling
#if defined(QTB_DEV_TOUCHPANEL)
  // viewport event
  bool viewportEvent(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // widget leave event
  void leaveEvent(QEvent *event);

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
#if defined(QTB_DEV_DESKTOP) && !QTB_TEST_TOUCHPANEL_ON_DESKTOP
  void wheelEvent(QWheelEvent *event);
#endif // defined(QTB_DEV_DESKTOP) && !QTB_TEST_TOUCHPANEL_ON_DESKTOP

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

#if QTB_DRAG_AND_DROP_SUPPORT
  // drag and drop
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
#endif // QTB_DRAG_AND_DROP_SUPPORT

  // viewport size hint
  QSize viewportSizeHint() const;

private:
  // convert to desktop
  bool convertToDesktop(QPoint &point);

#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)

private:
  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // desktop panel
  DesktopPanel *desktopPanel;

  // key buffer
  KeyBuffer *keyBuffer;

#if defined(QTB_DEV_TOUCHPANEL)
  // for software button
  QRect softwareButtonRect;

  // for software keyboard
  QRect softwareKeyboardRect;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSVIEW_H
