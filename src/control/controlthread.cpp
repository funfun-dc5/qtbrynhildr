// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// controlthread.cpp

// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QSize>

// Local Header
#include "controlthread.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
#if defined(QTB_RECORDER)
ControlThread::ControlThread(Settings *settings, MainWindow *mainWindow, Recorder *recorder)
#else  // defined(QTB_RECORDER)
ControlThread::ControlThread(Settings *settings, MainWindow *mainWindow)
#endif // defined(QTB_RECORDER)
  :
  NetThread("ControlThread", settings, mainWindow),
#if defined(QTB_BRYNHILDR2_SUPPORT)
  serverVersion(SERVER_VERSION_BRYNHILDR2),
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  keyBuffer(0),
  mouseBuffer(0),
#if defined(QTB_RECORDER)
  recorder(recorder),
#endif // defined(QTB_RECORDER)
  monitorCount(0),
  sentMode(0)
{
  outputLog = false; // for DEBUG

  // keyboard buffer
  keyBuffer = mainWindow->getKeyBuffer();

  // mouse buffer
  mouseBuffer = mainWindow->getMouseBuffer();
}

// destructor
ControlThread::~ControlThread()
{
  // delete objects
  if (com_data != 0){
	delete com_data;
	com_data = 0;
  }
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
	  if (keyInfo->keycode == VK_SHIFT ||
		  keyInfo->keycode == VK_MENU  ||
		  keyInfo->keycode == VK_CONTROL){
		// toggle status
		keydown = (keydown == KEYDOWN_ON) ? KEYDOWN_OFF : KEYDOWN_ON;
	  }

	  // set key information
	  com_data->keycode = keyInfo->keycode;
	  com_data->keycode_flg = keyInfo->keycode_flg;
	}

	// set keydown flag
	com_data->keydown = keydown;
  }
  else {
	// NOT under control
	// Nothing to do
  }

#if 0 // for DEBUG
  cout << "keycode     = " << hex << (int)com_data->keycode << endl;
  cout << "keycode_flg = " << hex << (int)com_data->keycode_flg << endl;
  cout << "keydown     = " << hex << (int)com_data->keydown << endl << flush;
#endif

#if defined(QTB_BRYNHILDR2_SUPPORT)
  // for GamePad
#if defined(Q_OS_WIN)
  if (settings->getOnSupportGamePad()){
	JOYINFOEX gamepad_btn;

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
	  com_data->gamepad1 = 32768;
	  com_data->gamepad2 = 32768;
	  com_data->gamepad3 = 32768;
	  com_data->gamepad4 = 32768;
	  com_data->gamepad5 = 65535;
	  com_data->gamepad6 = 0;
	  com_data->gamepad7 = 0;
	  com_data->gamepad8 = 0;
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
#endif // defined(Q_OS_WIN)

#endif // defined(QTB_BRYNHILDR2_SUPPORT)

#if 0 // for DEBUG
  static bool oneFlag = true;
  if (oneFlag){
	oneFlag = false;
	printHeader();
  }
#endif

#if defined(QTB_RECORDER)
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
#endif // defined(QTB_RECORDER)

  // save mode
  sentMode = com_data->mode;

  // send header for next communication
  long dataSize;
  dataSize = sendData(sock_control, (char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " sent data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

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
	switch(com_data->mode){
	case MODE_RESULT_PASSWORD_ERROR:
	  // password error
	  return PROCESS_PASSWORD_ERROR;
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
	  cout << "Unknown Error...: com_data->mode = " << com_data->mode << endl << flush;
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
#if defined(QTB_BRYNHILDR2_SUPPORT)
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
		// change to Qt::CrossCursor for Brynhildr (>= 2.0.0)
		// change mouse cursor
		emit changeMouseCursor(Qt::ArrowCursor);
	  }
	}
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
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
  // Nothing to do
  return TRANSMIT_SUCCEEDED;
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
  keydown = KEYDOWN_OFF;

  // succeeded to connect
  settings->setConnected(true);
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
  com_data->thread		= THREAD_CONTROL;
  com_data->mode		= MODE_PUBLIC;
  com_data->monitor_no	= settings->getMonitorNo();
#if defined(QTB_BRYNHILDR2_SUPPORT)
  if (serverVersion == SERVER_VERSION_BRYNHILDR2){
	com_data->mouse_cursor = settings->getOnDisplayCursor() ? MOUSE_CURSOR_ON : MOUSE_CURSOR_OFF;
  }
#endif // defined(QTB_BRYNHILDR2_SUPPORT)


  // for control
  com_data->control		= settings->getOnControl() ? CONTROL_ON : CONTROL_OFF;
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
  com_data->sound_type		= settings->getOnSound() ? SOUNT_TYPE_ON : SOUND_TYPE_OFF;
  com_data->sound_capture	= settings->getSoundCapture();
  com_data->sound_quality	= settings->getSoundQuality();
}

} // end of namespace qtbrynhildr
