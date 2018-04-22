// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "common/netthread.h"
#if QTB_RECORDER
#include "function/recorder.h"
#endif // QTB_RECORDER
#include "keybuffer.h"
#include "mousebuffer.h"
#include "settings.h"
#if QTB_PUBLIC_MODE6_SUPPORT
#include "parameters.h"
#include "windows/ntfs.h"
#endif // QTB_PUBLIC_MODE6_SUPPORT

namespace qtbrynhildr {

// ControlThread
class ControlThread : public NetThread
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // desktop panel
  DesktopPanel *desktopPanel;

  // server version
  SERVER_VERSION serverVersion;

#if QTB_PUBLIC_MODE6_SUPPORT
  // current mode
  PUBLICMODE_VERSION currentMode;
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // SHIFT key status
  KEYDOWN keydownSHIFT;
  // ALT key status
  KEYDOWN keydownALT;
  // CONTROL key status
  KEYDOWN keydownCONTROL;

  // monitor count
  MONITOR_COUNT monitorCount;

  // sent mode
  MODE sentMode;

  // done check password flag
  bool doneCheckPassword;

#if QTB_PUBLIC_MODE7_SUPPORT
#define QTB_ICON_WIDTH		32
#define QTB_ICON_HEIGHT		32
#define QTB_ICON_SIZE		(QTB_ICON_WIDTH*QTB_ICON_HEIGHT)
#define QTB_ICON_IMAGE_SIZE	(QTB_ICON_SIZE*4)

  // mouse cursor image data (4096 bytes * 2)
  uchar andMaskImage[QTB_ICON_IMAGE_SIZE];
  uchar xorMaskImage[QTB_ICON_IMAGE_SIZE];

#if !defined(Q_OS_WIN)
  // cursor point color
  QRgb cursorPointColor;
#endif // !defined(Q_OS_WIN)
#endif // QTB_PUBLIC_MODE7_SUPPORT

#if QTB_PUBLIC_MODE6_SUPPORT
  // local buffer
  char *buffer;

  // clipboard top address
  char *clipboardTop;

  // transfer file progress
  qint64 transferFileProgress;
  // send file progress unit
  qint64 transferFileProgressUnit;

  // ntfs utility
  NTFS *ntfs;
#endif // QTB_PUBLIC_MODE6_SUPPORT

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
#if QTB_RECORDER
ControlThread(Settings *settings, DesktopPanel *desktopPanel, Recorder *recorder);
#else  // QTB_RECORDER
ControlThread(Settings *settings, DesktopPanel *desktopPanel);
#endif // QTB_RECORDER
  // destructor
  ~ControlThread();

#if QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)
  // set cursor point color
  void setCursorPointColor(QRgb cursorPointColor);
#endif // QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)

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

  // set mouse control
  void setMouseControl();

  // set keyboard control
  void setKeyboardControl();

  // set gamepad control
  void setGamePadControl();

#if QTB_PUBLIC_MODE6_SUPPORT
  // send clipboard
  bool sendClipboard();

  // receive clipboard
  bool receiveClipboard();

  // send file
  bool sendFile();

  // sreceive file
  bool receiveFile();
#endif // QTB_PUBLIC_MODE6_SUPPORT

#if QTB_PUBLIC_MODE7_SUPPORT
  // receive mouse cursor image
  bool receiveMouseCursorImage();

  // check color mouse cursor image
  bool isColorMouseCursorImage(uchar *image, int size);

  // change mouse cursor
  void changeMouseCursor();

  // create color mouse cursor
  QCursor createColorMouseCursor(uchar *image, uchar *mask);

  // create monochrome mouse cursor
  QCursor createMonochromeMouseCursor(uchar *image, uchar *mask);
#endif // QTB_PUBLIC_MODE7_SUPPORT

signals:
  // connected
  void connected();

  // refresh window
  void refreshWindow();

  // refresh menu
  void refreshMenu();

  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);

  // exit application
  void exitApplication();

#if QTB_PUBLIC_MODE6_SUPPORT
  // set clipboard
  void setClipboard(QString clipboardString);

  // set progress bar value for transfer file
  void setFileTransferProgressBarValue(int value);
#endif // QTB_PUBLIC_MODE6_SUPPORT
};

} // end of namespace qtbrynhildr

#endif // CONTROLTHREAD_H
