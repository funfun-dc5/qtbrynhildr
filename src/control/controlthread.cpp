// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if QTB_PUBLIC_MODE6_SUPPORT
#include <fstream>
#include <iostream>
#endif // QTB_PUBLIC_MODE6_SUPPORT

// Qt Header
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QByteArray>
#include <QFileInfo>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#include <QSize>

// Local Header
#include "controlthread.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
#if QTB_RECORDER
ControlThread::ControlThread(Settings *settings, MainWindow *mainWindow, Recorder *recorder)
#else  // QTB_RECORDER
ControlThread::ControlThread(Settings *settings, MainWindow *mainWindow)
#endif // QTB_RECORDER
  :
  NetThread("ControlThread", settings, mainWindow),
#if QTB_BRYNHILDR2_SUPPORT
  serverVersion(SERVER_VERSION_BRYNHILDR2),
#endif // QTB_BRYNHILDR2_SUPPORT
  keyBuffer(0),
  mouseBuffer(0),
#if QTB_RECORDER
  recorder(recorder),
#endif // QTB_RECORDER
  monitorCount(0),
  sentMode(0)
{
  outputLog = false; // for DEBUG

  // keyboard buffer
  keyBuffer = mainWindow->getKeyBuffer();

  // mouse buffer
  mouseBuffer = mainWindow->getMouseBuffer();

  // initialize key and mouse information
  keydownSHIFT	= KEYDOWN_OFF;
  keydownALT	= KEYDOWN_OFF;
  keydownCONTROL= KEYDOWN_OFF;
  prevPos.x = 0;
  prevPos.y = 0;

  // done check password flag
  doneCheckPassword = false;

#if QTB_PUBLIC_MODE6_SUPPORT
  // local buffer
  buffer = new char [QTB_CONTROL_LOCAL_BUFFER_SIZE+2];

  // clipboard top address
  clipboardTop = &buffer[16];

  // transfer file progress
  transferFileProgress = 0;
  // transfer file progress unit
  transferFileProgressUnit = 0;

  // NTFS utility
  ntfs = new NTFS();
#endif // QTB_PUBLIC_MODE6_SUPPORT
}

// destructor
ControlThread::~ControlThread()
{
  // delete objects
  if (com_data != 0){
	delete com_data;
	com_data = 0;
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
	clipboardTop = 0;
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// connect to server
CONNECT_RESULT ControlThread::connectToServer()
{
  if (sock_control != INVALID_SOCKET)
	return CONNECT_SUCCEEDED;

  // connect
  sock_control = socketToServer();
  if (sock_control == INVALID_SOCKET){
	return CONNECT_FAILED;
  }

#if 0 // for TEST
  int ret,flag;
  flag = 1;
  ret = setsockopt(sock_control, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
  if (ret == -1){
	cout << "Couldn't setsockopt(TCP_NODELAY)" << endl << flush;
  }
#endif // for TEST
#if 0 // for TEST
  int ret,sock_buf_size;
  sock_buf_size = 0;
  ret = setsockopt(sock_control, SOL_SOCKET, SO_SNDBUF, (char *)&sock_buf_size, sizeof(sock_buf_size));
  if (ret == -1){
	cout << "Failed: setsockopt()" << endl << flush;
  }
#endif // for TEST

  // create objects
  // header
  if (com_data == 0){
	com_data = new COM_DATA;
  }

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT ControlThread::processForHeader()
{
  // initialize header(com_data)
  initHeader();

  // for control
  if (settings->getOnControl()){
	//-------------------------------------------
	// 1) Mouse Control
	//-------------------------------------------
	// mouse info (button, wheel)
	MouseInfo *mouseInfo = mouseBuffer->get();
	if (mouseInfo != 0){
	  switch(mouseInfo->type){
	  case TYPE_MOUSE_RIGHT_BUTTON:
		com_data->mouse_right = mouseInfo->value.button;
		break;
	  case TYPE_MOUSE_LEFT_BUTTON:
		com_data->mouse_left = mouseInfo->value.button;
		break;
#if QTB_EXTRA_BUTTON_SUPPORT
	  case TYPE_MOUSE_MIDDLE_BUTTON:
		if (settings->getOnExtraButtonSupport()){
		  com_data->mouse_middle = mouseInfo->value.button;
		}
		break;
	  case TYPE_MOUSE_BACK_BUTTON:
		if (settings->getOnExtraButtonSupport()){
		  com_data->mouse_x1 = mouseInfo->value.button;
		}
		break;
	  case TYPE_MOUSE_FORWARD_BUTTON:
		if (settings->getOnExtraButtonSupport()){
		  com_data->mouse_x2 = mouseInfo->value.button;
		}
		break;
#endif // QTB_EXTRA_BUTTON_SUPPORT
	  case TYPE_MOUSE_WHEEL:
		com_data->mouse_wheel = mouseInfo->value.wheel;
		break;
	  default:
		// unknown type
		ABORT();
		break;
	  } // end of switch
	}
	// mouse position
	MOUSE_POS pos = mouseBuffer->getMousePos();
	// if mouse cursor is moved.
	if (prevPos.x != pos.x || prevPos.y != pos.y || settings->getOnHoldMouseControl()){
	  // set information
	  com_data->mouse_move = MOUSE_MOVE_ON;
	  if (QTB_DESKTOP_IMAGE_SCALING){
		QSize windowSize = mainWindow->getSize();
		QSize desktopSize = mainWindow->getDesktopSize();
		if (windowSize.width() < 0 || windowSize.height() < 0 ||
			desktopSize.width() < 0 || desktopSize.height() < 0){
		  // Nothing to do
		  com_data->mouse_move = MOUSE_MOVE_OFF;
		}
		else {
		  if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
			qreal scalingFactor = settings->getDesktopScalingFactorForZoom();
			if (scalingFactor > 1.0){
			  com_data->mouse_x = pos.x * desktopSize.width()/windowSize.width() * scalingFactor
								+ settings->getDesktopOffsetX();
			  com_data->mouse_y = pos.y * desktopSize.height()/windowSize.height() * scalingFactor
								+ settings->getDesktopOffsetY();
			}
			else {
			  com_data->mouse_x = pos.x * desktopSize.width()/windowSize.width()
				+ settings->getDesktopOffsetX();
			  com_data->mouse_y = pos.y * desktopSize.height()/windowSize.height()
				+ settings->getDesktopOffsetY();
			}
		  }
		  else {
			com_data->mouse_x = pos.x * desktopSize.width()/windowSize.width()
			  + settings->getDesktopOffsetX();
			com_data->mouse_y = pos.y * desktopSize.height()/windowSize.height()
			  + settings->getDesktopOffsetY();
		  }
		}
	  }
	  else {
		com_data->mouse_x = pos.x + settings->getDesktopOffsetX();
		com_data->mouse_y = pos.y + settings->getDesktopOffsetY();
	  }

	  // save prevPos
	  prevPos = pos;
	}

	//-------------------------------------------
	// 2) Keyboard Control
	//-------------------------------------------
	KeyInfo *keyInfo = keyBuffer->get();
	if (keyInfo != 0){
	  // check shift/alt/control status
	  if (keyInfo->keycode == VK_SHIFT	||
		  keyInfo->keycode == VK_RSHIFT	||
		  keyInfo->keycode == VK_LSHIFT){
		// toggle status
		keydownSHIFT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }
	  if (keyInfo->keycode == VK_MENU  ||
		  keyInfo->keycode == VK_RMENU ||
		  keyInfo->keycode == VK_LMENU){
		// toggle status
		keydownALT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }
	  if (keyInfo->keycode == VK_CONTROL ||
		  keyInfo->keycode == VK_RCONTROL||
		  keyInfo->keycode == VK_LCONTROL){
		// toggle status
		keydownCONTROL = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }

	  // set keydown
	  com_data->keydown = KEYDOWN_ON;

	  // set key information
	  com_data->keycode = keyInfo->keycode;
	  com_data->keycode_flg = keyInfo->keycode_flg;
	}

	// shift/alt/control
	if (keydownSHIFT || keydownALT || keydownCONTROL){
	  // set keydown
	  com_data->keydown = KEYDOWN_ON;
	}
  }
  else {
	// NOT under control
	// Nothing to do
  }

#if 0 // for DEBUG
  ios::fmtflags flags = cout.flags();
  cout << "keycode     = " << hex << (int)com_data->keycode << endl;
  cout << "keycode_flg = " << hex << (int)com_data->keycode_flg << endl;
  cout << "keydown     = " << hex << (int)com_data->keydown << endl << flush;
  cout.flags(flags);
#endif

#if QTB_BRYNHILDR2_SUPPORT
  // for GamePad
  if (settings->getOnSupportGamePad()){
	setGamePadControl();
  }
#endif // QTB_BRYNHILDR2_SUPPORT

#if 0 // for DEBUG
  static bool oneFlag = true;
  if (oneFlag){
	oneFlag = false;
	printHeader();
  }
#endif

#if QTB_RECORDER
  // replaying
  if (settings->getOnReplayingControl()){
	// replay
	COM_DATA *recordedCOM_DATA = recorder->getCOM_DATA();
	if (recordedCOM_DATA != 0){
	  // override com_data
	  memcpy(com_data, recordedCOM_DATA, sizeof(COM_DATA));
	}
	else {
	  // stop replaying
	  if (settings->getOnExitAfterReplay()){
		emit exitApplication();
	  }
	  // refresh menu
	  emit refreshMenu();
	}
  }
  // recording
  else if (settings->getOnRecordingControl()){
	recorder->putCOM_DATA(com_data);
  }
#endif // QTB_RECORDER

  // save mode
  sentMode = com_data->mode;

  // send header for next communication
  long dataSize;
  dataSize = sendHeader(sock_control, (char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " sent data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

#if QTB_PUBLIC_MODE6_SUPPORT
  // send clilpboard/file
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// cliboard
	if (com_data->data_type == DATA_TYPE_CLIPBOARD &&
		!settings->getOnDisableTransferClipboard()){
	  // send clipboard
	  if (!sendClipboard()){
		// failed to send clipboard
		//		cout << "Failed to send clipboard!" << endl << flush;
		return PROCESS_RESTART;
	  }
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 !settings->getOnDisableTransferFile()){
	  // send file
	  keyBuffer->setEnabled(false); // disabled keyboard
	  mouseBuffer->setEnabled(false); // disabled mouse
	  bool result = sendFile();
	  keyBuffer->setEnabled(true);
	  mouseBuffer->setEnabled(true);
	  if (!result){
		// failed to send file
		//		cout << "Failed to send file!" << endl << flush;
		return PROCESS_RESTART;
	  }
	}
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT

  // receive header
  dataSize = receiveData(sock_control, (char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

#if 0 // for DEBUG
  static bool twoFlag = true;
  if (twoFlag){
	twoFlag = false;
	printHeader();
  }
#endif

  // check result
  if (com_data->mode != sentMode){
    switch((int)com_data->mode){
	case MODE_RESULT_PASSWORD_ERROR:
	  if (doneCheckPassword){
		// network error
		return PROCESS_NETWORK_ERROR;
	  }
	  else {
		// password error
		return PROCESS_PASSWORD_ERROR;
	  }
	  break;
	case MODE_RESULT_CONNECT_ERROR:
	  // connect error
	  return PROCESS_CONNECT_ERROR;
	  break;
	case MODE_RESULT_CONNECT_WAIT:
	  // wait for connect
	  return PROCESS_SUCCEEDED;
	  break;
	default:
	  // unknown error
	  cout << "Unknown Error...: com_data->mode = " << (int)com_data->mode << endl << flush;
	  return PROCESS_UNKNOWN_ERROR;
	  break;
	}
  }
  else {
	// No Error
	// check server_cx, server_cy
	//	cout << "[ControlThread] server_cx = " << com_data->server_cx << endl << flush;
	//	cout << "[ControlThread] server_cy = " << com_data->server_cy << endl << flush;
	settings->setDesktopWidth(com_data->server_cx);
	settings->setDesktopHeight(com_data->server_cy);
	// checked password
	doneCheckPassword = true;

#if QTB_BRYNHILDR2_SUPPORT
	if (settings->getOnDisableBrynhildr2Support()){
	  // same as Brynhildr (<= 1.1.5)
	  com_data->server_version = SERVER_VERSION_BRYNHILDR;
	}
	// save server version
	if (serverVersion != com_data->server_version){
	  serverVersion = com_data->server_version;
	  if (serverVersion == SERVER_VERSION_BRYNHILDR){
		// change to Qt::CrossCursor for Brynhildr (<= 1.1.5)
		// change mouse cursor
		emit changeMouseCursor(Qt::CrossCursor);
	  }
	  else if (serverVersion == SERVER_VERSION_BRYNHILDR2){
		// change to Qt::ArrowCursor for Brynhildr (>= 2.0.0)
		// change mouse cursor
		emit changeMouseCursor(Qt::ArrowCursor);
	  }
	}
#endif // QTB_BRYNHILDR2_SUPPORT
  }

  // check monitor no
  if (monitorCount != com_data->monitor_count){
	monitorCount = com_data->monitor_count;
	//	cout << "[ControlThread] monitor_count=" << (int)monitorCount << endl << flush;
	settings->setMonitorCount(monitorCount);
	emit refreshMenu();
  }

  // counter up
  if (counter_control < 5){
	counter_control++;
  }

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT ControlThread::transmitBuffer()
{
#if QTB_PUBLIC_MODE6_SUPPORT
  // receive clilpboard/file
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// cliboard
	if (com_data->data_type == DATA_TYPE_CLIPBOARD &&
		!settings->getOnDisableTransferClipboard()){
	  // receive clipboard
	  if (!receiveClipboard()){
		// failed to transfer clipboard
		//		cout << "Failed to receive clipboard!" << endl << flush;
		return TRANSMIT_RESTART;
	  }
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 !settings->getOnDisableTransferFile()){
	  // receive file
	  keyBuffer->setEnabled(false); // disabled keyboard
	  mouseBuffer->setEnabled(false); // disabled mouse
	  bool result = receiveFile();
	  keyBuffer->setEnabled(true);
	  mouseBuffer->setEnabled(true);
	  if (!result){
		// failed to receive file
		//		cout << "Failed to receive file!" << endl << flush;
		return TRANSMIT_RESTART;
	  }
	}
  }
  return TRANSMIT_SUCCEEDED;
#else // QTB_PUBLIC_MODE6_SUPPORT
  // Nothing to do
  return TRANSMIT_SUCCEEDED;
#endif // QTB_PUBLIC_MODE6_SUPPORT
}

// connected
void ControlThread::connectedToServer()
{
  // reset counter
  counter_control = 0;

  // mouse position
  prevPos.x = -1;
  prevPos.y = -1;

  // shift/alt/control status
  keydownSHIFT	= KEYDOWN_OFF;
  keydownALT	= KEYDOWN_OFF;
  keydownCONTROL= KEYDOWN_OFF;

  NetThread::connectedToServer();

  // succeeded to connect
  settings->setConnected(true);
  // connected
  emit connected();
  // refresh window
  emit refreshWindow();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void ControlThread::shutdownConnection()
{
  if (sock_control != INVALID_SOCKET){
	shutdown(sock_control, SD_BOTH);
	closesocket(sock_control);
	sock_control = INVALID_SOCKET;
  }
  if (sock_graphics != INVALID_SOCKET){
	shutdown(sock_graphics, SD_BOTH);
	closesocket(sock_graphics);
	sock_graphics = INVALID_SOCKET;
  }
  if (sock_sound != INVALID_SOCKET){
	shutdown(sock_sound, SD_BOTH);
	closesocket(sock_sound);
	sock_sound = INVALID_SOCKET;
  }

  // done check password flag
  if (!runThread){
	doneCheckPassword = false;
  }

  NetThread::shutdownConnection();
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// init protocol header
void ControlThread::initHeader()
{
  // zero clear
  memset(com_data, 0, sizeof(COM_DATA));

  // set protocol version string
  memcpy(com_data->ver,
		 PROTOCOL_VERSION_STRING, PROTOCOL_VERSION_STRING_LENGTH);

  // common
  com_data->data_type	= DATA_TYPE_DATA;
#if QTB_PUBLIC_MODE6_SUPPORT
  int sendFileCount = settings->getSendFileCount();
  if (settings->getOnSendClipboard()){
	com_data->data_type	= DATA_TYPE_CLIPBOARD;
	// set data size
	com_data->data_size = settings->getSendClipboardString().size() * 2 + 16;
  }
  else if (sendFileCount > 0){
	com_data->data_type	= DATA_TYPE_FILE;
	// set data size
	QFileInfo fileInfo((QString)settings->getSendFileNames().at(sendFileCount-1));
	qint64 fileSize = fileInfo.size();
	com_data->data_size = (SIZE64)fileSize;
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT
  com_data->thread		= THREAD_CONTROL;
#if QTB_PUBLIC_MODE6_SUPPORT
  com_data->mode		= settings->getPublicModeVersion();
#else // QTB_PUBLIC_MODE6_SUPPORT
  com_data->mode		= MODE_PUBLIC5;
#endif // QTB_PUBLIC_MODE6_SUPPORT
  com_data->monitor_no	= settings->getMonitorNo();
#if QTB_BRYNHILDR2_SUPPORT
  if (serverVersion == SERVER_VERSION_BRYNHILDR2){
	com_data->mouse_cursor = settings->getOnDisplayCursor() ? MOUSE_CURSOR_ON : MOUSE_CURSOR_OFF;
  }
#endif // QTB_BRYNHILDR2_SUPPORT

  // for control
  com_data->control		= settings->getOnControl() ? CONTROL_ON : CONTROL_OFF;
#if QTB_PLUGINS_DISABLE_SUPPORT
  // for plugins disable
  com_data->plugins_disable = settings->getOnPluginsDisable() ? PLUGINS_DISABLE_ON : PLUGINS_DISABLE_OFF;
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  com_data->mouse_move	= MOUSE_MOVE_OFF;
  com_data->mouse_x		= (POS)0;
  com_data->mouse_y		= (POS)0;
  com_data->mouse_left	= (POS)0;
  com_data->mouse_right	= (POS)0;
  com_data->mouse_wheel	= (MOUSE_WHEEL)0;
  com_data->keycode		= (char)VK_NONE_00;
  com_data->keycode_flg	= KEYCODE_FLG_KEYUP;
  com_data->keydown		= KEYDOWN_OFF;

  // for graphics
  com_data->zoom			= (ZOOM)1.0;
  if (settings->getOnGraphics()){
	// Graphics ON
	if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
	  if (settings->getDesktopScalingFactor() != 1.0){
		com_data->zoom	= (ZOOM)settings->getDesktopScalingFactorForZoom();
	  }
	}
	com_data->image_cx			= (SIZE)settings->getDesktopWidth();
	com_data->image_cy			= (SIZE)settings->getDesktopHeight();
	com_data->client_scroll_x	= (POS)settings->getDesktopOffsetX();
	com_data->client_scroll_y	= (POS)settings->getDesktopOffsetY();
	com_data->video_quality		= settings->getVideoQuality();
  }
  else {
	// Graphics OFF
	com_data->image_cx			= (SIZE)settings->getDesktopWidth();
	com_data->image_cy			= (SIZE)settings->getDesktopHeight();
	//	com_data->image_cx			= (SIZE)8;
	//	com_data->image_cy			= (SIZE)8;
	com_data->client_scroll_x	= 0;
	com_data->client_scroll_y	= 0;
	com_data->video_quality		= VIDEO_QUALITY_MINIMUM;
  }

  // for sound
#if QTB_CELT_SUPPORT
  if (settings->getOnDisableBrynhildr2Support() ||
	  serverVersion == SERVER_VERSION_BRYNHILDR){
	// for Brynhildr (<= 1.1.5) : PCM only
	com_data->sound_type = settings->getOnSound() ? SOUND_TYPE_PCM : SOUND_TYPE_OFF;
  }
  else {
	// for Brynhildr (>= 2.0.0) : soundType (PCM/CELT/...)
	com_data->sound_type = settings->getOnSound() ? settings->getSoundType() : SOUND_TYPE_OFF;
  }
#else // QTB_CELT_SUPPORT
  com_data->sound_type		= settings->getOnSound() ? SOUND_TYPE_PCM : SOUND_TYPE_OFF;
#endif // QTB_CELT_SUPPORT
  com_data->sound_capture	= settings->getSoundCapture();
  com_data->sound_quality	= settings->getSoundQuality();
}

// set gamepad control info.
#if QTB_BRYNHILDR2_SUPPORT
#if defined(Q_OS_WIN)
void ControlThread::setGamePadControl()
{
  JOYINFOEX gamepad_btn;

  // check server version and
  if (serverVersion < SERVER_VERSION_BRYNHILDR2 ||
	  settings->getOnDisableBrynhildr2Support()){
	// Nothing to do
	return;
  }

  gamepad_btn.dwFlags = JOY_RETURNALL;
  gamepad_btn.dwSize = sizeof(JOYINFOEX);

  // get status of GamePad(1)
  MMRESULT result = ::joyGetPosEx(JOYSTICKID1,&gamepad_btn);

  // set information
  if (result == JOYERR_NOERROR){
	// found gamepad
	com_data->gamepad1 = gamepad_btn.dwXpos;
	com_data->gamepad2 = gamepad_btn.dwYpos;
	com_data->gamepad3 = gamepad_btn.dwZpos;
	com_data->gamepad4 = gamepad_btn.dwRpos;
	com_data->gamepad5 = gamepad_btn.dwPOV;
	com_data->gamepad6 = gamepad_btn.dwButtons;
	com_data->gamepad7 = gamepad_btn.dwUpos;
	com_data->gamepad8 = gamepad_btn.dwVpos;
  }
  else if (result == MMSYSERR_NODRIVER){
	// NOT exist driver for gamepad
  }
  else if (result == MMSYSERR_INVALPARAM){
	// invalid parameter
	// internal error
  }
  else if (result == MMSYSERR_BADDEVICEID){
	// bad device id for gamepad
	// internal error
  }
  else if (result == JOYERR_UNPLUGGED){
	// not found gamepad
	com_data->gamepad1 = 0x7FFF;
	com_data->gamepad2 = 0x7FFF;
	com_data->gamepad3 = 0x7FFF;
	com_data->gamepad4 = 0x7FFF;
	com_data->gamepad5 = 0xFFFF;
	com_data->gamepad6 = 0x0000;
	com_data->gamepad7 = 0x0000;
	com_data->gamepad8 = 0x0000;
  }
  else {
	// Unknown Error
	com_data->gamepad1 = 0;
	com_data->gamepad2 = 0;
	com_data->gamepad3 = 0;
	com_data->gamepad4 = 0;
	com_data->gamepad5 = 0;
	com_data->gamepad6 = 0;
	com_data->gamepad7 = 0;
	com_data->gamepad8 = 0;
  }
}
#else // defined(Q_OS_WIN)
void ControlThread::setGamePadControl()
{
  // Yet
}
#endif // defined(Q_OS_WIN)

#endif // QTB_BRYNHILDR2_SUPPORT

#if QTB_PUBLIC_MODE6_SUPPORT

// send clipboard
bool ControlThread::sendClipboard()
{
  QString clipboardString = settings->getSendClipboardString();
  SIZE stringSize = clipboardString.size() * 2;
  SIZE sentDataSize = 0;
#if _MSC_VER
  char *localBuffer = new char[stringSize + 16 + 2];
#else // _MSC_VER
  char localBuffer[stringSize + 16 + 2];
#endif // _MSC_VER

  // check
  if (stringSize == 0){
	// Nothing to do
	return true;
  }

  //  cout << "sendClipboard = " << qPrintable(clipboardString) << endl << flush;
  //  cout << "sendClipboard.size = " << stringSize << endl << flush;

  // copy to local buffer and send to server
  memset(localBuffer, 0, stringSize + 16 + 2);
  memcpy(&localBuffer[16], clipboardString.unicode(), stringSize);
  sentDataSize = sendData(sock_control, localBuffer, stringSize + 16);
  stringSize -= sentDataSize - 16;

  // flag clear
  settings->setOnSendClipboard(false);

#if _MSC_VER
  if (localBuffer != 0)
	delete [] localBuffer;
#endif // _MSC_VER

  // check result
  if (stringSize == 0){
	return true;
  }
  else {
	return false;
  }
}

// receive clipboard
bool ControlThread::receiveClipboard()
{
  SIZE clipboardSize = com_data->data_size;
  SIZE receivedDataSize = 0;

  // get cliboard
  while(clipboardSize > QTB_CONTROL_LOCAL_BUFFER_SIZE){
	receivedDataSize = receiveData(sock_control, buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	clipboardSize -= receivedDataSize;
  }
  if (clipboardSize > 0){
	receivedDataSize = receiveData(sock_control, buffer, clipboardSize);
	clipboardSize -= receivedDataSize;
	buffer[receivedDataSize] = '\0';
	buffer[receivedDataSize+1] = '\0';
  }
  if (clipboardSize == 0){
	// set cliboard
	QString clipboardString = QString((const QChar *)clipboardTop, -1);
	//	cout << "receiveClipboard = " << qPrintable(clipboardString) << endl << flush;
	emit setClipboard(clipboardString);
	return true;
  }
  else {
	return false;
  }
}

// send file
bool ControlThread::sendFile()
{
  char filename[260+1] = {0};
  char fileTimeStamp[24+1] = {0};
  static int previousProgress = -1;

  qint64 fileSize = (qint64)com_data->data_size;
  qint64 fileSizeOrg = fileSize;
  qint64 sentDataSizeTotal = 0;
  qint64 sentDataSize = 0;

  // initialize progress bar
  if (settings->getOnShowTotalProgressForTransferFile()){
	if (transferFileProgress == 0){
	  transferFileProgressUnit = 100/settings->getSendFileCount();
	  previousProgress = -1;
	}
  }
  else {
	transferFileProgressUnit = 100;
	previousProgress = -1;
  }

  // send filename
  int sendFileCount = settings->getSendFileCount() - 1;
  QString fileName = settings->getSendFileNames().at(sendFileCount);
  QFileInfo fileInfo(fileName);
  strncpy(filename, qPrintable(fileInfo.fileName()), 260);
  sentDataSize = sendData(sock_control, filename, 260);

  // send time stamp
  qint64 CreationTime = ntfs->toFILETIME(fileInfo.created()); // UTC
  fileTimeStamp[0] = (CreationTime >>  0) & 0xFF;
  fileTimeStamp[1] = (CreationTime >>  8) & 0xFF;
  fileTimeStamp[2] = (CreationTime >> 16) & 0xFF;
  fileTimeStamp[3] = (CreationTime >> 24) & 0xFF;
  fileTimeStamp[4] = (CreationTime >> 32) & 0xFF;
  fileTimeStamp[5] = (CreationTime >> 40) & 0xFF;
  fileTimeStamp[6] = (CreationTime >> 48) & 0xFF;
  fileTimeStamp[7] = (CreationTime >> 56) & 0xFF;
  qint64 LastAccessTime = ntfs->toFILETIME(fileInfo.lastRead()); // UTC
  fileTimeStamp[8]  = (LastAccessTime >>  0) & 0xFF;
  fileTimeStamp[9]  = (LastAccessTime >>  8) & 0xFF;
  fileTimeStamp[10] = (LastAccessTime >> 16) & 0xFF;
  fileTimeStamp[11] = (LastAccessTime >> 24) & 0xFF;
  fileTimeStamp[12] = (LastAccessTime >> 32) & 0xFF;
  fileTimeStamp[13] = (LastAccessTime >> 40) & 0xFF;
  fileTimeStamp[14] = (LastAccessTime >> 48) & 0xFF;
  fileTimeStamp[15] = (LastAccessTime >> 56) & 0xFF;
  qint64 LastWriteTime = ntfs->toFILETIME(fileInfo.lastModified()); // UTC
  fileTimeStamp[16] = (LastWriteTime >>  0) & 0xFF;
  fileTimeStamp[17] = (LastWriteTime >>  8) & 0xFF;
  fileTimeStamp[18] = (LastWriteTime >> 16) & 0xFF;
  fileTimeStamp[19] = (LastWriteTime >> 24) & 0xFF;
  fileTimeStamp[20] = (LastWriteTime >> 32) & 0xFF;
  fileTimeStamp[21] = (LastWriteTime >> 40) & 0xFF;
  fileTimeStamp[22] = (LastWriteTime >> 48) & 0xFF;
  fileTimeStamp[23] = (LastWriteTime >> 56) & 0xFF;
  sentDataSize = sendData(sock_control, fileTimeStamp, 24);

  // send file image
  fstream file;
  file.open(qPrintable(fileName), ios::in | ios::binary);
  if (file.is_open()){
	while(fileSize > QTB_CONTROL_LOCAL_BUFFER_SIZE){
	  // read to buffer
	  file.read(buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	  // send to server
	  sentDataSize = sendData(sock_control, buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	  sentDataSizeTotal += sentDataSize;
	  fileSize -= sentDataSize;
	  // check for cancel file transferring
	  if (!runThread){
		runThread = true;
		// cancel
		fileSize = -1;
		break;
	  }
	  // set progress bar
	  transferFileProgress = transferFileProgressUnit*(float)sentDataSizeTotal/fileSizeOrg;
	  if (previousProgress != transferFileProgress){
		//		cout << "transferFileProgress: " << transferFileProgress << endl << flush;
		previousProgress = transferFileProgress;
		emit setFileTransferProgressBarValue(transferFileProgress);
	  }
	}
	if (fileSize > 0){
	  int fragmentSize = fileSize & 0x3FF; // fileSize % 1024(10bit)
	  int paddingSize = (fragmentSize < 16) ? 16 - fragmentSize : 0;
	  // read to buffer
	  file.read(buffer, fileSize);
	  // send to server
	  sentDataSize = sendData(sock_control, buffer, fileSize + paddingSize);
	  sentDataSize -= paddingSize;
	  sentDataSizeTotal += sentDataSize;
	  fileSize -= sentDataSize;
	  // set progress bar
	  transferFileProgress = transferFileProgressUnit*(float)sentDataSizeTotal/fileSizeOrg;
	  //	  cout << "transferFileProgress: " << transferFileProgress << endl << flush;
	  emit setFileTransferProgressBarValue(transferFileProgress);
	}
	file.close();
  }
#if 0 // for DEBUG
  else {
	cout << "open error!" << endl << flush;
  }
#endif // for DEBUG

  // sent a file
  settings->setSendFileCount(sendFileCount);
  //  cout << "SendFile = " << qPrintable(fileName) << endl << flush;
  //  cout << "SendFileCount = " << settings->getSendFileCount() << endl << flush;

  // reset progress bar
  if (settings->getOnShowTotalProgressForTransferFile()){
	if (sendFileCount == 0){
	  transferFileProgress = 0;
	  emit setFileTransferProgressBarValue(100);
	}
  }
  else {
	  transferFileProgress = 0;
	  emit setFileTransferProgressBarValue(100);
  }

  // check result
  if (fileSize == 0){
	return true;
  }
  else {
	return false;
  }
}

// receive file
bool ControlThread::receiveFile()
{
  char filename[260+1];
  char fileTimeStamp[24+1]; // dummy read
  static int previousProgress = -1;

  qint64 fileSize = (qint64)com_data->data_size;
  qint64 fileSizeOrg = fileSize;
  qint64 receivedDataSizeTotal = 0;
  qint64 receivedDataSize = 0;

  // initialize progress bar
  if (transferFileProgress == 0){
	transferFileProgressUnit = 100;
	previousProgress = -1;
  }

  // get filename
  receivedDataSize = receiveData(sock_control, filename, 260);

  // get file time stamp
  receivedDataSize = receiveData(sock_control, fileTimeStamp, 24);

  // get file image
  fstream file;
  QString localFilename = settings->getOutputPath() + filename;
  file.open(qPrintable(localFilename), ios::out | ios::binary);
  if (file.is_open()){
	while(fileSize > QTB_CONTROL_LOCAL_BUFFER_SIZE){
	  receivedDataSize = receiveData(sock_control, buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	  if (receivedDataSize > 0){
		file.write(buffer, receivedDataSize);
		receivedDataSizeTotal += receivedDataSize;
		fileSize -= receivedDataSize;
		// check for cancel file transferring
		if (!runThread){
		  runThread = true;
		  // cancel
		  fileSize = -1;
		  break;
		}
		// set progress bar
		transferFileProgress = transferFileProgressUnit*(float)receivedDataSizeTotal/fileSizeOrg;
		if (previousProgress != transferFileProgress){
		  previousProgress = transferFileProgress;
		  emit setFileTransferProgressBarValue(transferFileProgress);
		}
	  }
	}
	if (fileSize > 0){
	  receivedDataSize = receiveData(sock_control, buffer, fileSize);
	  if (receivedDataSize > 0){
		file.write(buffer, receivedDataSize);
		receivedDataSizeTotal += receivedDataSize;
		fileSize -= receivedDataSize;
		// set progress bar
		transferFileProgress = transferFileProgressUnit*(float)receivedDataSizeTotal/fileSizeOrg;
		emit setFileTransferProgressBarValue(transferFileProgress);
	  }
	}
	file.close();
  }

  // reset progress bar
  transferFileProgress = 0;
  emit setFileTransferProgressBarValue(100);

  // check result
  if (fileSize == 0){
	return true;
  }
  else {
	return false;
  }
}

#endif // QTB_PUBLIC_MODE6_SUPPORT

} // end of namespace qtbrynhildr
