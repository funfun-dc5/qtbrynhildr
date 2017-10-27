// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOPWINDOW_H
#define DESKTOPWINDOW_H
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
#include <QSize>
#include <QTextStream>
#include <QWheelEvent>
#include <QWidget>

// Local Header
#include "control/keybuffer.h"
#include "control/mousebuffer.h"
#include "settings.h"
#include "windows/eventconverter.h"

namespace qtbrynhildr {

class QtBrynhildr;

// DesktopWindow
#if defined(Q_OS_WIN)
class DesktopWindow : public QWidget, public QAbstractNativeEventFilter
#else // defined(Q_OS_WIN)
class DesktopWindow : public QWidget
#endif // defined(Q_OS_WIN)
{
  Q_OBJECT

public:
  // constructor
  DesktopWindow(Settings *settings, QtBrynhildr *qtbrynhildr);
  // destructor
  ~DesktopWindow();

  // set event converter
  void setEventConverter(EventConverter *eventConverter);

  // get keyboard buffer
  KeyBuffer *getKeyBuffer() const;

  // get mouse buffer
  MouseBuffer *getMouseBuffer() const;

  // refresh desktop window
  void refreshDesktop(QImage image);

  // resize window
  void resizeWindow();

  // clear desktop window
  void clearDesktop();

  // get window size
  QSize getSize() const;

  // get desktop window size
  QSize getDesktopSize() const;

  // set height of menu bar
  void setHeightOfMenuBar(int heightOfMenuBar)
  {
	this->heightOfMenuBar = heightOfMenuBar;
  }

  // height of status bar
  void setHeightOfStatusBar(int heightOfStatusBar)
  {
	this->heightOfStatusBar = heightOfStatusBar;
  }

  // set full screen flag
  void setOnFullScreen(bool onFullScreen);

  // set draw marker counter
  void setDrawMarkerCounter(int drawMarkerCounter)
  {
	this->drawMarkerCounter = drawMarkerCounter;
  }

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

private:
#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)

  // open keyboard log file
  bool openKeyboardLogFile(QString filename);

  // close keyboard log file
  bool closeKeyboardLogFile();

  // print mouse button event
  void printMouseButtonEvent(QMouseEvent *event);

  // set mouse button event
  void setMouseButtonEvent(QMouseEvent *event, MouseInfoValue value);

  // scroll area
  bool scrollArea(uchar VK_Code, bool onKeyPress);

  // get desktop scaling factor
  qreal getDesktopScalingFactor(QSize targetSize);

  // get height of menu bar
  int getHeightOfMenuBar()
  {
	return heightOfMenuBar;
  }

  // get height of status bar
  int getHeightOfStatusBar()
  {
	return heightOfStatusBar;
  }

private:
  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

  // image
  QImage image;

  // window size
  QSize currentSize;

  // previous window size
  QSize previousSize;

  // real desktop size
  QSize desktopSize;

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

  // height of menu bar
  int heightOfMenuBar;

  // height of status bar
  int heightOfStatusBar;

  // full screen flag
  bool onFullScreen;

  // draw marker counter
  int drawMarkerCounter;

#if defined(Q_OS_OSX)
  // previous KEYCODE_FLG
  KEYCODE_FLG previous_KEYCODE_FLG;
#endif // defined(Q_OS_OSX)

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
};

} // end of namespace qtbrynhildr

#endif // DESKTOPWINDOW_H
