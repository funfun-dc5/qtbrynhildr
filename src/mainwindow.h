// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// Common Header
#include "common/common.h"

// Qt Header
#if defined(Q_OS_WIN)
#include <QAbstractNativeEventFilter>
#endif // defined(Q_OS_WIN)
#include <QEvent>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QSize>
#include <QWheelEvent>

// Local Header
#include "control/keybuffer.h"
#include "control/mousebuffer.h"
#include "settings.h"
#include "windows/eventconverter.h"

namespace qtbrynhildr {

// MainWindow
#if defined(Q_OS_WIN)
class MainWindow : public QWidget, public QAbstractNativeEventFilter
#else // defined(Q_OS_WIN)
class MainWindow : public QWidget
#endif // defined(Q_OS_WIN)
{
  Q_OBJECT

public:
  // constructor
  MainWindow(Settings *settings, QWidget *parent);
  // destructor
  ~MainWindow();

  // set event converter
  void setEventConverter(EventConverter *eventConverter);

  // get keyboard buffer
  KeyBuffer *getKeyBuffer() const;

  // get mouse buffer
  MouseBuffer *getMouseBuffer() const;

  // refresh desktop window
  void refreshDesktop(QImage image);

  // refresh desktop window
  void refreshDesktop();

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

  // minimum size hint
  QSize minimumSizeHint() const;
  // size hint
  QSize sizeHint() const;

protected:
  // for event handling
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

private:
#if defined(Q_OS_WIN)
  // native event filter
  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif // defined(Q_OS_WIN)

  // print mouse button event
  void printMouseButtonEvent(QMouseEvent *event);

  // set mouse button event
  void setMouseButtonEvent(QMouseEvent *event, MouseInfoValue value);

  // scroll area
  bool scrollArea(uchar VK_Code, bool onKeyPress);

  // get desktop scaling factor
  qreal getDesktopScalingFactor(QSize targetSize);

  // get height of menu bar
  int getHeightOfMenuBar();

  // get height of status bar
  int getHeightOfStatusBar();

private:
  // settings
  Settings *settings;

  // parent
  QWidget *parent;

  // image
  QImage image;

  // window size
  QSize size;

  // previous window size
  QSize previousSize;

  // real desktop size
  QSize desktopSize;

  // event converter
  EventConverter *eventConverter;

  // keyboard buffer
  KeyBuffer *keyBuffer;

  // mouse buffer
  MouseBuffer *mouseBuffer;

  // output log flag
  bool outputLog;

  // output log flag for keyboard
  bool outputLogForKeyboard;

  // output log flag for mouse
  bool outputLogForMouse;

  // height of menu bar
  int heightOfMenuBar;

  // height of status bar
  int heightOfStatusBar;

  // height of menu bar in hiding
  int heightOfMenuBarInHiding;

  // height of status bar in hiding
  int heightOfStatusBarInHiding;
};

} // end of namespace qtbrynhildr

#endif // MAINWINDOW_H
