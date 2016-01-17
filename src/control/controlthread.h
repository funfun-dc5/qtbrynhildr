// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// controlthread.h

#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#if !QTB_BRYNHILDR2_SUPPORT
#include <QCursor>
#endif // !QTB_BRYNHILDR2_SUPPORT

// Local Header
#include "common/netthread.h"
#if QTB_RECORDER
#include "function/recorder.h"
#endif // QTB_RECORDER
#include "keybuffer.h"
#include "mainwindow.h"
#include "mousebuffer.h"
#include "settings.h"

namespace qtbrynhildr {

// ControlThread
class ControlThread : public NetThread
{
  Q_OBJECT

public:
  // constructor
#if QTB_RECORDER
ControlThread(Settings *settings, MainWindow *mainWindow, Recorder *recorder);
#else  // QTB_RECORDER
ControlThread(Settings *settings, MainWindow *mainWindow);
#endif // QTB_RECORDER
  // destructor
  ~ControlThread();

protected:
  // connect to server
  CONNECT_RESULT connectToServer();

  // process for header
  PROCESS_RESULT processForHeader();

  // transmit local buffer to global buffer
  TRANSMIT_RESULT transmitBuffer();

  // connected
  void connectedToServer();

  // shutdown connection
  void shutdownConnection();

private:
  // initialize protocol header
  void initHeader();

  // set gamepad control
  void setGamePadControl();

private:
#if QTB_BRYNHILDR2_SUPPORT
  // server version
  SERVER_VERSION serverVersion;
#endif // QTB_BRYNHILDR2_SUPPORT

  // keyboard buffer
  KeyBuffer *keyBuffer;

  // mouse buffer
  MouseBuffer *mouseBuffer;

#if QTB_RECORDER
  // recorder
  Recorder *recorder;
#endif // QTB_RECORDER

  // mouse position
  MOUSE_POS prevPos;

  // SHIFT/ALT/CONTROL key status
  KEYDOWN keydown;

  // monitor count
  MONITOR_COUNT monitorCount;

  // sent mode
  MODE sentMode;

signals:
  // refresh window
  void refreshWindow();

  // refresh menu
  void refreshMenu();

#if QTB_BRYNHILDR2_SUPPORT
  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);
#endif // QTB_BRYNHILDR2_SUPPORT

  // exit application
  void exitApplication();
};

} // end of namespace qtbrynhildr

#endif // CONTROLTHREAD_H
