// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

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
#include "parameters.h"
#include "windows/ntfs.h"

namespace qtbrynhildr {

// filename image size
#define QTB_FILENAME_IMAGE_SIZE 260
// timestamp image size
#define QTB_TIMESTAMP_IMAGE_SIZE 24

// icon information
#define QTB_ICON_WIDTH		32
#define QTB_ICON_HEIGHT		32
#define QTB_ICON_SIZE		(QTB_ICON_WIDTH*QTB_ICON_HEIGHT)
#define QTB_ICON_IMAGE_SIZE	(QTB_ICON_SIZE*4)

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

  // current mode
  PUBLICMODE_VERSION currentMode;

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

  // mouse cursor image data (4096 bytes * 2)
  uchar andMaskImage[QTB_ICON_IMAGE_SIZE];
  uchar xorMaskImage[QTB_ICON_IMAGE_SIZE];

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
  // cursor point color
  QRgb cursorPointColor;
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

  // transfer file progress
  qint64 transferFileProgress;

  // send file progress unit
  qint64 transferFileProgressUnit;

  // ntfs utility
  NTFS *ntfs;

  // maxfps flag
  bool onMaxfps;

  // clipboard top address
  char *clipboardTop;

  // local buffer
  char *buffer;

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

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
  // set cursor point color
  void setCursorPointColor(QRgb cursorPointColor);
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

  // set maxfps flag
  void setOnMaxfps(bool onMaxfps)
  {
	this->onMaxfps = onMaxfps;
  }

  // get maxfps flag
  bool getOnMaxfps() const
  {
	return onMaxfps;
  }

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
  // send eader
  inline long sendHeader(const char *buf, long size);

  // send data
  inline long sendData(const char *buf, long size);

  // receive data
  inline long receiveData(char *buf, long size);

  // setup header
  void setupHeader();

  // initialize protocol header
  void initHeader();

  // initialize protocol header for common
  void initHeaderForCommon();

  // initialize protocol header for control
  void initHeaderForControl();

  // initialize protocol header for graphics
  void initHeaderForGraphics();

  // initialize protocol header for sound
  void initHeaderForSound();

  // set mouse control
  void setMouseControl();

  // set keyboard control
  void setKeyboardControl();

  // set gamepad control
  void setGamePadControl();

  // check server version
  void checkServerVersion();

  // check mode
  void checkMode();

#if QTB_RECORDER
  // recorder function
  void recordAndReplayHeader();
#endif // QTB_RECORDER

  // send to server
  bool sendToServer();

  // receive from server
  bool receiveFromServer();

  // send clipboard
  bool sendClipboard();

  // receive clipboard
  bool receiveClipboard();

  // send file
  bool sendFile();

  // sreceive file
  bool receiveFile();

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

signals:
  // refresh menu
  void refreshMenu();

  // change mouse cursor
  void changeMouseCursor(const QCursor &cursor);

  // exit application
  void exitApplication();

  // set clipboard
  void setClipboard(QString clipboardString);

  // set progress bar value for transfer file
  void setFileTransferProgressBarValue(int value);
};

} // end of namespace qtbrynhildr

#endif // CONTROLTHREAD_H
