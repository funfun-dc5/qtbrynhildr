// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPPANEL_H
#define DESKTOPPANEL_H
// Common Header
#include "common/common.h"

// Qt Header
#if defined(Q_OS_WIN)
#include <QAbstractNativeEventFilter>
#endif // defined(Q_OS_WIN)
#include <QEvent>
#if QTB_DRAG_AND_DROP_SUPPORT
#include <QDragEnterEvent>
#include <QDropEvent>
#endif // QTB_DRAG_AND_DROP_SUPPORT
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPoint>
#include <QSize>
#include <QTextStream>
#include <QWheelEvent>
#include <QWidget>

// Local Header
#include "controlthread/keybuffer.h"
#include "controlthread/mousebuffer.h"
#include "settings.h"
#include "windows/eventconverter.h"

namespace qtbrynhildr {

class QtBrynhildr;

// DesktopPanel
class DesktopPanel
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
protected:
  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // image
  QImage image;

  // desktop size
  QSize currentSize;

  // previous desktop size
  QSize previousSize;

  // event converter
  EventConverter *eventConverter;

  // on shift key
  bool onShiftKey;

  // keyboard buffer
  KeyBuffer *keyBuffer;

  // mouse buffer
  MouseBuffer *mouseBuffer;

  // current mouse position
  QPoint currentMousePos;

  // full screen flag
  bool onFullScreen;

  // draw marker counter
  int drawMarkerCounter;

  // previous KEYCODE_FLG
  KEYCODE_FLG previous_KEYCODE_FLG;

  // margins
  int widthMargin;
  int heightMargin;

  // keyboard log file
  QFile	*keyboardLogFile;

  // keyboard log file stream
  QTextStream *keyboardLogFileStream;

  // output log flag for keyboard
  bool outputLogForKeyboard;

  // output log flag for mouse
  bool outputLogForMouse;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  DesktopPanel(QtBrynhildr *qtbrynhildr);
  // destructor
  virtual ~DesktopPanel();

  // set event converter
  void setEventConverter(EventConverter *eventConverter);

  // get keyboard buffer
  KeyBuffer *getKeyBuffer() const;

  // get mouse buffer
  MouseBuffer *getMouseBuffer() const;

  // set mouse position
  void setMousePos(POS x, POS y);

  // refresh desktop window
  void refreshDesktop(QImage &image);

  // resize window
  void resizeWindow();

  // clear desktop window
  void clearDesktop();

  // get size
  QSize getSize() const;

  // get window size
  QSize getWindowSize() const;

  // set margins
  void setMargins(int widthMargin, int heightMargin)
  {
	this->widthMargin = widthMargin;
	this->heightMargin = heightMargin;
  }

  // set full screen flag
  void setOnFullScreen(bool onFullScreen);

  // set draw marker counter
  void setDrawMarkerCounter(int drawMarkerCounter)
  {
	this->drawMarkerCounter = drawMarkerCounter;
  }

  // resize desktop
  virtual void resizeDesktop(int width, int height) = 0;

  // update desktop
  virtual void updateDesktop() = 0;

  // get current mouse position
  QPoint getCurrentMousePos() const
  {
	return currentMousePos;
  }

#if QTB_SOFTWARE_KEYBOARD_AND_BUTTON
  // mouse move
  void mouseMove(QPoint mousePos, bool marker = true);
  // mouse move relatively
  void mouseMoveRelatively(QPoint mousePos, bool marker = true);
#endif // QTB_SOFTWARE_KEYBOARD_AND_BUTTON

  // for event handling
#if QTB_TOUCHPANEL_WINDOW
public:
#else // QTB_TOUCHPANEL_WINDOW
protected:
#endif // QTB_TOUCHPANEL_WINDOW

#if defined(QTB_DEV_TOUCHPANEL)
  // event
  bool event(QEvent *event);
#endif // defined(QTB_DEV_TOUCHPANEL)

  // mouse event 
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  // move mouse cursor
  void moveMouseCursor(QMouseEvent *event, bool marker = true);

  // keyboard event
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

#if QTB_DRAG_AND_DROP_SUPPORT
  // drag and drop
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
#endif // QTB_DRAG_AND_DROP_SUPPORT

#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)

private:
  // open keyboard log file
  bool openKeyboardLogFile(QString filename);

  // close keyboard log file
  bool closeKeyboardLogFile();

  // print mouse button event
  void printMouseButtonEvent(QMouseEvent *event);

  // set mouse button event
  void setMouseButtonEvent(QMouseEvent *event, MOUSE_BUTTON value);

  // scroll area
  bool scrollArea(uchar VK_Code, bool onKeyPress);
};

} // end of namespace qtbrynhildr

#endif // DESKTOPPANEL_H
