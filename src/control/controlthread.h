// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// controlthread.h

#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#if !defined(QTB_BRYNHILDR2_SUPPORT)
#include <QCursor>
#endif // !defined(QTB_BRYNHILDR2_SUPPORT)

// Local Header
#include "common/netthread.h"
#if defined(QTB_RECORDER)
#include "function/recorder.h"
#endif // defined(QTB_RECORDER)
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
#if defined(QTB_RECORDER)
ControlThread(Settings *settings, MainWindow *mainWindow, Recorder *recorder);
#else  // defined(QTB_RECORDER)
ControlThread(Settings *settings, MainWindow *mainWindow);
#endif // defined(QTB_RECORDER)
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
#if defined(QTB_BRYNHILDR2_SUPPORT)
  // server version
  SERVER_VERSION serverVersion;
#endif // defined(QTB_BRYNHILDR2_SUPPORT)

  // keyboard buffer
  KeyBuffer *keyBuffer;

  // mouse buffer
  MouseBuffer *mouseBuffer;

#if defined(QTB_RECORDER)
  // recorder
  Recorder *recorder;
#endif // defined(QTB_RECORDER)

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

#if defined(QTB_BRYNHILDR2_SUPPORT)
  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);
#endif // defined(QTB_BRYNHILDR2_SUPPORT)

  // exit application
  void exitApplication();
};

} // end of namespace qtbrynhildr

#endif // CONTROLTHREAD_H
