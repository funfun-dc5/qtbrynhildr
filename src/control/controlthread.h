// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

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
#if QTB_PUBLIC_MODE6_SUPPORT
#include "parameters.h"
#endif // QTB_PUBLIC_MODE6_SUPPORT
#include "settings.h"
#if QTB_PUBLIC_MODE6_SUPPORT
#include "windows/ntfs.h"
#endif // QTB_PUBLIC_MODE6_SUPPORT

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

  // receive mouse cursor image
  bool receiveMouseCursorImage();

  //  convert Big Endian 2 bytes
  void convertBE2bytes(BYTE *ptr)
  {
	qint16 value;

	value = *((unsigned short*)ptr);
	*ptr++ = (BYTE)(value >> 8) & 0xFF;
	*ptr   = (BYTE)(value >> 0) & 0xFF;
  }

  //  convert Big Endian 4 bytes
  void convertBE4bytes(BYTE *ptr)
  {
	qint32 value;

	value = *((unsigned int*)ptr);
	*ptr++ = (BYTE)(value >> 24) & 0xFF;
	*ptr++ = (BYTE)(value >> 16) & 0xFF;
	*ptr++ = (BYTE)(value >> 8)  & 0xFF;
	*ptr   = (BYTE)(value >> 0)  & 0xFF;
  }

  //  convert Big Endian 8 bytes
  void convertBE8bytes(BYTE *ptr)
  {
	qint64 value;

	value = *((unsigned int*)ptr);
	*ptr++ = (BYTE)(value >> 56) & 0xFF;
	*ptr++ = (BYTE)(value >> 48) & 0xFF;
	*ptr++ = (BYTE)(value >> 40) & 0xFF;
	*ptr++ = (BYTE)(value >> 32) & 0xFF;
	*ptr++ = (BYTE)(value >> 24) & 0xFF;
	*ptr++ = (BYTE)(value >> 16) & 0xFF;
	*ptr++ = (BYTE)(value >> 8)  & 0xFF;
	*ptr   = (BYTE)(value >> 0)  & 0xFF;
  }

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

signals:
  // connected
  void connected();

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

#if QTB_PUBLIC_MODE6_SUPPORT
  // set clipboard
  void setClipboard(QString clipboardString);

  // set progress bar value for transfer file
  void setFileTransferProgressBarValue(int value);
#endif // QTB_PUBLIC_MODE6_SUPPORT

private:
  // done check password flag
  bool doneCheckPassword;

#if QTB_PUBLIC_MODE6_SUPPORT
private:
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
};

} // end of namespace qtbrynhildr

#endif // CONTROLTHREAD_H
