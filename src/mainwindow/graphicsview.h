// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017-2018 FunFun <fu.aba.dc5@gmail.com>

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
#include <QPoint>
#include <QSize>
#include <QRect>

// Local Header
#include "controlthread/keybuffer.h"
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

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
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

#if defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW
  // scaling factor
  qreal scalingFactor;
  // scaling factor for fullscreen
  qreal scalingFactorForFullScreen;
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  GraphicsView(QGraphicsScene *scene, QtBrynhildr *qtbrynhildr, QWidget *parent = Q_NULLPTR);
  // destructor
  ~GraphicsView();

  // size hint
  QSize sizeHint() const;

  // scale
  void setScale(qreal scalingFactor);

#if defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW
  void setScalingFactorForFullScreen(qreal scalingFactorForFullScreen)
  {
	this->scalingFactorForFullScreen = scalingFactorForFullScreen;
  }
#endif // defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
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
  void wheelEvent(QWheelEvent *event);

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
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSVIEW_H
