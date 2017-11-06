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
#include <QDir>
#include <QFileInfo>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if QTB_PUBLIC_MODE7_SUPPORT
#include <QBitmap>
#include <QCursor>
#include <QImage>
#endif // QTB_PUBLIC_MODE7_SUPPORT
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
ControlThread::ControlThread(Settings *settings, DesktopPanel *desktopPanel, Recorder *recorder)
#else  // QTB_RECORDER
ControlThread::ControlThread(Settings *settings, DesktopPanel *desktopPanel)
#endif // QTB_RECORDER
  :
  NetThread("ControlThread", settings, desktopPanel),
  serverVersion(SERVER_VERSION_BRYNHILDR2),
  currentMode(0),
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
  keyBuffer = desktopPanel->getKeyBuffer();

  // mouse buffer
  mouseBuffer = desktopPanel->getMouseBuffer();

  // initialize key and mouse information
  keydownSHIFT	= KEYDOWN_OFF;
  keydownALT	= KEYDOWN_OFF;
  keydownCONTROL= KEYDOWN_OFF;
  prevPos.x = 0;
  prevPos.y = 0;

  // done check password flag
  doneCheckPassword = false;

#if QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)
  // cursor point color
  cursorPointColor = 0xFFFFFFFF;
#endif // QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)

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
#if QTB_PUBLIC_MODE6_SUPPORT
  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
	clipboardTop = 0;
  }
#endif // QTB_PUBLIC_MODE6_SUPPORT
}

#if QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)
// set cursor point color
void ControlThread::setCursorPointColor(QRgb cursorPointColor)
{
  if (this->cursorPointColor != cursorPointColor){
	//	cout << "cursorPointColor: 0x" << hex << cursorPointColor << endl << flush;
	this->cursorPointColor = cursorPointColor;
	changeMouseCursor();
  }
}
#endif // QTB_PUBLIC_MODE7_SUPPORT && !defined(Q_OS_WIN)

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
	  case TYPE_MOUSE_FILEDROP:
		com_data->mouse_left = mouseInfo->value.button;
		com_data->filedrop = FILEDROP_ON;
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
		QSize windowSize = desktopPanel->getSize();
		QSize desktopSize = desktopPanel->getDesktopSize();
		if (!(windowSize.isValid() && desktopSize.isValid())){
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
#if QTB_DESKTOP_COMPRESS_MODE // for TEST
	  // desktop compress mode
	  int desktopCompressMode = settings->getDesktopCompressMode();
	  if (desktopCompressMode > 1){
		com_data->mouse_x *= desktopCompressMode;
		com_data->mouse_y *= desktopCompressMode;
	  }
#endif // for TEST

	  // save prevPos
	  prevPos = pos;
	}

	//-------------------------------------------
	// 2) Keyboard Control
	//-------------------------------------------
	KeyInfo *keyInfo = keyBuffer->get();
	if (keyInfo != 0){
	  // check shift/alt/control status
#if 1 // for TEST
	  switch((int)keyInfo->keycode){
	  case VK_SHIFT:
	  case VK_RSHIFT:
	  case VK_LSHIFT:
		// toggle status
		keydownSHIFT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
		break;
	  case VK_MENU:
	  case VK_RMENU:
	  case VK_LMENU:
		// toggle status
		keydownALT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
		break;
	  case VK_CONTROL:
	  case VK_RCONTROL:
	  case VK_LCONTROL:
		// toggle status
		keydownCONTROL = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
		break;
	  }
#else // for TEST
	  if (keyInfo->keycode == VK_SHIFT	||
		  keyInfo->keycode == VK_RSHIFT	||
		  keyInfo->keycode == VK_LSHIFT){
		// toggle status
		keydownSHIFT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }
	  else
	  if (keyInfo->keycode == VK_MENU  ||
		  keyInfo->keycode == VK_RMENU ||
		  keyInfo->keycode == VK_LMENU){
		// toggle status
		keydownALT = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }
	  else
	  if (keyInfo->keycode == VK_CONTROL ||
		  keyInfo->keycode == VK_RCONTROL||
		  keyInfo->keycode == VK_LCONTROL){
		// toggle status
		keydownCONTROL = keyInfo->keycode_flg == KEYCODE_FLG_KEYDOWN;
	  }
#endif // for TEST

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

  // for GamePad
  if (settings->getOnGamePadSupport()){
	setGamePadControl();
  }

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
		settings->getOnTransferClipboardSupport()){
	  // send clipboard
	  if (!sendClipboard()){
		// failed to send clipboard
		//		cout << "Failed to send clipboard!" << endl << flush;
		return PROCESS_RESTART;
	  }
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 settings->getOnTransferFileSupport()){
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

	if (!settings->getOnBrynhildr2Support()){
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
		if (settings->getOnDisplayMouseCursor()){
		  emit changeMouseCursor(Qt::CrossCursor);
		}
		else {
		  emit changeMouseCursor(Qt::ArrowCursor);
		}
	  }
	}
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

#if QTB_PUBLIC_MODE7_SUPPORT
  if (currentMode == 0){
	// save current mode
	currentMode = settings->getPublicModeVersion();
  }
  else if (currentMode != settings->getPublicModeVersion()){
	// save current mode
	currentMode = settings->getPublicModeVersion();
	if (currentMode == PUBLICMODE_VERSION7){
		if (settings->getOnDisplayMouseCursor()){
		  emit changeMouseCursor(Qt::CrossCursor);
		}
		else {
		  emit changeMouseCursor(Qt::ArrowCursor);
		}
	}
	else {
	  // to Cross Cursor
	  emit changeMouseCursor(Qt::CrossCursor);
	}
	// refresh menu
	emit refreshMenu();
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

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
		settings->getOnTransferClipboardSupport()){
	  // receive clipboard
	  if (!receiveClipboard()){
		// failed to transfer clipboard
		//		cout << "Failed to receive clipboard!" << endl << flush;
		return TRANSMIT_RESTART;
	  }
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 settings->getOnTransferFileSupport()){
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
#if QTB_PUBLIC_MODE7_SUPPORT
	// mouse cursor image
	else if (com_data->data_type == DATA_TYPE_DATA){
	  // receive mouse cursor image
	  bool result = receiveMouseCursorImage();
	  if (!result){
		// failed to receive mouse cursor image
		//		cout << "Failed to receive mouse cursor image!" << endl << flush;
		return TRANSMIT_RESTART;
	  }
	}
#endif // QTB_PUBLIC_MODE7_SUPPORT
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
  if (serverVersion == SERVER_VERSION_BRYNHILDR2){
	com_data->mouse_cursor = settings->getOnDisplayMouseCursor() ? MOUSE_CURSOR_ON : MOUSE_CURSOR_OFF;
	if (settings->getOnShowSoftwareButton())
	  com_data->mouse_cursor = MOUSE_CURSOR_ON;
  }

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
	com_data->image_cx			= (SIZE)settings->getDesktopWidth();
	com_data->image_cy			= (SIZE)settings->getDesktopHeight();
	com_data->client_scroll_x	= (POS)settings->getDesktopOffsetX();
	com_data->client_scroll_y	= (POS)settings->getDesktopOffsetY();
	com_data->video_quality		= settings->getVideoQuality();
	// scaling
	if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
	  if (settings->getDesktopScalingFactor() <= 1.0){
		// scale down
		com_data->zoom	= (ZOOM)settings->getDesktopScalingFactorForZoom();
	  }
#if 0 // defined(QTB_DEV_TOUCHPANEL) // 0 for TEST
	  else {
		// scale up
		com_data->image_cx *= settings->getDesktopScalingFactorForZoom();
		com_data->image_cy *= settings->getDesktopScalingFactorForZoom();
	  }
#endif // defined(QTB_DEV_TOUCHPANEL)
	}
#if QTB_DESKTOP_COMPRESS_MODE // for TEST
	// desktop compress mode
	if (settings->getDesktopCompressMode() > 1)
	  com_data->zoom *= settings->getDesktopCompressMode();
#endif // QTB_DESKTOP_COMPRESS_MODE
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
  // max fps
  com_data->max_fps = (char)settings->getFrameRate();

  // for sound
#if QTB_CELT_SUPPORT
  if (!settings->getOnBrynhildr2Support() ||
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
#if defined(Q_OS_WIN)
void ControlThread::setGamePadControl()
{
  JOYINFOEX gamepad_btn;

  // check server version and
  if (serverVersion < SERVER_VERSION_BRYNHILDR2 ||
	  !settings->getOnBrynhildr2Support()){
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

#if QTB_PUBLIC_MODE7_SUPPORT
// receive mouse cursor image
bool ControlThread::receiveMouseCursorImage()
{
  qint64 receivedDataSize = 0;

  // get AND Mask Cursor
  receivedDataSize = receiveData(sock_control, (char*)andMaskImage, 4096);
  if (receivedDataSize != 4096){
	return false;
  }
#if 0 // for TEST
  else {
	fstream file;
	file.open("jpg/andMaskImage.bin", ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write((char*)andMaskImage, receivedDataSize);
	  file.close();
	}
	QImage image(andMaskImage, 32, 32, QImage::Format_RGBA8888);
	image = image.mirrored(false, true);
	image.save("jpg/andMaskImage.bmp", "BMP");
  }
#endif // for TEST

  // get XOR Mask Cursor
  receivedDataSize = receiveData(sock_control, (char*)xorMaskImage, 4096);
  if (receivedDataSize != 4096){
	return false;
  }
#if 0 // for TEST
  else {
	fstream file;
	file.open("jpg/xorMaskImage.bin", ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write((char*)xorMaskImage, receivedDataSize);
	  file.close();
	}
	QImage image(xorMaskImage, 32, 32, QImage::Format_RGBA8888);
	image = image.mirrored(false, true);
	image.save("jpg/xorMaskImage.bmp", "BMP");
  }
#endif // for TEST

  if (!settings->getOnDisplayMouseCursor()){
	// BGRA -> RGBA
	for(int i = 0; i < 4096; i += 4){
	  uchar r, g, b;
	  // and
	  b = andMaskImage[i];
	  g = andMaskImage[i+1];
	  r = andMaskImage[i+2];
	  andMaskImage[i]   = r;
	  andMaskImage[i+1] = g;
	  andMaskImage[i+2] = b;
	  // xor
	  b = xorMaskImage[i];
	  g = xorMaskImage[i+1];
	  r = xorMaskImage[i+2];
	  xorMaskImage[i]   = r;
	  xorMaskImage[i+1] = g;
	  xorMaskImage[i+2] = b;
	}

	// change mouse cursor
	changeMouseCursor();
  }

  return true;
}

// change mouse cursor
void ControlThread::changeMouseCursor()
{
  if (settings->getPublicModeVersion() < PUBLICMODE_VERSION7)
	return;

  QCursor newCursor;
  if (isColorMouseCursorImage(xorMaskImage, andMaskImage, 4096)){
	newCursor = createColorMouseCursor(xorMaskImage);
  }
  else {
	newCursor = createMonochromeMouseCursor(xorMaskImage, andMaskImage);
  }
  // change mouse cursor
  emit changeMouseCursor(newCursor);
}

// check color mouse cursor image
bool ControlThread::isColorMouseCursorImage(uchar *image, uchar *mask, int size)
{
  bool maskAllZero = true;

  for(int i = 0; i < size; i += 4){
	if ((image[i]   != 0 && image[i]   != 0xFF) || // R
		(image[i+1] != 0 && image[i+1] != 0xFF) || // G
		(image[i+2] != 0 && image[i+2] != 0xFF) || // B
		 image[i+3] != 0 ){ // A
	  //	  cout << "Found Color Mouse Cursor!" << endl << flush;
	  return true;
	}
	if (mask[i] != 0 || mask[i+1] != 0 || mask[i+2] != 0)
	  maskAllZero = false;
  }

  if (maskAllZero){
	//	cout << "Found Color Mouse Cursor! (mask = 0)" << endl << flush;
	return true;
  }
  else {
	//	cout << "Found Monochrome Mouse Cursor!" << endl << flush;
	return false;
  }
}

// create color mouse cursor
QCursor ControlThread::createColorMouseCursor(uchar *image)
{
  // Cursor Image
  QImage cursorImage(image, 32, 32, QImage::Format_RGBA8888);
  cursorImage = cursorImage.mirrored(false, true);
  // cursorImage.save("jpg/cursorImage.bmp", "BMP");
  QPixmap cursor = QPixmap::fromImage(cursorImage, Qt::NoFormatConversion);

  // change mouse cursor image
  //  cursor.save("jpg/ZCursor_pixmap.bmp", "BMP");
  int hotX = (int)com_data->cursor_hotspot_x;
  int hotY = (int)com_data->cursor_hotspot_y;
  return QCursor(cursor, hotX, hotY);
}

// create monochrome mouse cursor
QCursor ControlThread::createMonochromeMouseCursor(uchar *image, uchar *mask)
{
  uchar bitmapImage[32*32*3];
  uchar maskImage[32*32*3];
  uchar *bitmaptop = bitmapImage;
  uchar *masktop = maskImage;

  // == Windows ==
  // XOR AND
  //  0   0     -> black
  //  1   0     -> white
  //  0   1     -> transparent
  //  1   1     -> Reverse screen

  // == Qt5 ==
  // B=0 and M=0 -> black
  // B=1 and M=0 -> white
  // B=1 and M=1 -> transparent
  // B=0 and M=1 -> XOR'd result under Windows.
  for (int i = 0; i < 4096; i++){
	if ((i+1) % 4 != 0){
	  if (image[i] == 0 && mask[i] == 0xFF){
		// transparent
		*bitmaptop++ = 0xFF;
		*masktop++ = 0xFF;
	  }
	  else if (image[i] == 0xFF && mask[i] == 0xFF){
		// XOR'd result under Windows.
		*bitmaptop++ = 0x00;
		*masktop++ = 0xFF;
	  }
	  else {
		*bitmaptop++ = image[i];
		*masktop++ = mask[i];
	  }
	}
  }

#if !defined(Q_OS_WIN) // for XOR'd all platforms
  uchar bitmapValue;
  if (((cursorPointColor & 0x00FF0000) >> 16) < 5 &&
	  ((cursorPointColor & 0x0000FF00) >> 8)  < 5 &&
	  ((cursorPointColor & 0x000000FF) >> 0)  < 5){
	// white
	bitmapValue = 0xFF;
  }
  else {
	// black
	bitmapValue = 0x00;
  }
  // convert to black or white
  for (int i = 0; i < 3072; i++){
	if (bitmapImage[i] == 0x00 && maskImage[i] == 0xFF){
	  bitmapImage[i] = bitmapValue;
	  maskImage[i] = 0x00;
	}
  }
#endif // !defined(Q_OS_WIN)

  QImage bitmapQImage(bitmapImage, 32, 32, QImage::Format_RGB888);
  bitmapQImage = bitmapQImage.mirrored(false, true);
  QBitmap bitmap = QBitmap::fromImage(bitmapQImage);
  QImage maskQImage(maskImage, 32, 32, QImage::Format_RGB888);
  maskQImage = maskQImage.mirrored(false, true);
  QBitmap maskBitmap = QBitmap::fromImage(maskQImage);
  int hotX = (int)com_data->cursor_hotspot_x;
  int hotY = (int)com_data->cursor_hotspot_y;
#if 0 // for TEST
  bitmap.save("jpg/ZCursor_bitmap.bmp", "BMP");
  maskBitmap.save("jpg/ZCursor_mask.bmp", "BMP");
#endif // for TEST

  return QCursor(bitmap, maskBitmap, hotX, hotY);
}
#endif // QTB_PUBLIC_MODE7_SUPPORT

} // end of namespace qtbrynhildr
