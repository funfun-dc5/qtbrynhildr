// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#define QTB_TEST 0

// Common Header
#include "common/common.h"

// System Header
#include <fstream>
#include <iostream>
#include <iomanip>

// #if defined(Q_OS_WIN)
// #include <windows.h>
// #endif // defined(Q_OS_WIN)

// Qt Header
#include <QBitmap>
#include <QByteArray>
#include <QCursor>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QSize>

// Local Header
#include "controlthread.h"
#include "qtbrynhildr.h"

// for TEST
#define TEST_THREAD							0
#define SAVE_MOUSE_CURSOR_IMAGE				0
#define SAVE_MOUSE_CURSOR_IMAGE_BINARY		0
#define HEXDUMP_MOUSE_CURSOR_IMAGE_BINARY	0

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
  :NetThread("ControlThread", settings)
  ,desktopPanel(desktopPanel)
  ,serverVersion(SERVER_VERSION_BRYNHILDR2)
  ,currentMode(0)
  ,keyBuffer(0)
  ,mouseBuffer(0)
#if QTB_RECORDER
  ,recorder(recorder)
#endif // QTB_RECORDER
  ,monitorCount(0)
  ,sentMode(0)
  ,doneCheckPassword(false)
  ,transferFileProgress(0)
  ,transferFileProgressUnit(0)
  ,ntfs(0)
  ,onMaxfps(true)
  ,clipboardTop(0)
  ,buffer(0)
{
  //outputLog = true; // for DEBUG

  // change thread sleep time
  //threadSleepTime = 10;

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

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
  // cursor point color
  cursorPointColor = 0xFFFFFFFF;
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

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
}

// destructor
ControlThread::~ControlThread()
{
  // delete objects
  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
	clipboardTop = 0;
  }

  // NTFS utility
  if (ntfs != 0){
	delete ntfs;
	ntfs = 0;
  }
}

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)
// set cursor point color
void ControlThread::setCursorPointColor(QRgb cursorPointColor)
{
  if (this->cursorPointColor != cursorPointColor){
	//	cout << "cursorPointColor: 0x" << hex << cursorPointColor << endl << flush;
	this->cursorPointColor = cursorPointColor;
	changeMouseCursor();
  }
}
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

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

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT ControlThread::processForHeader()
{
#if TEST_THREAD
  startTimeInfo();
#endif // TEST_THREAD

  // setup header
  setupHeader();

#if QTB_RECORDER
  recordAndReplayHeader();
#endif // QTB_RECORDER

  // save mode
  sentMode = com_data->mode;

  // send header for next communication
  long dataSize;
  dataSize = sendHeader((char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " sent data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got header");
#endif // TEST_THREAD

  // send clilpboard/file
  if (!sendToServer())
	return PROCESS_RESTART;

  // receive header
  dataSize = receiveData((char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got header");
#endif // TEST_THREAD

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
	  const QString text = QString("Unknown Error...: com_data->mode = ") +
		QString::number((int)com_data->mode);
	  emit outputLogMessage(PHASE_CONTROL, text);
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

	// check desktop size
	if (settings->getOnDesktopAutoresize() &&
		!doneCheckPassword && settings->getDesktopScalingFactor() == 1.0){
	  // cout << "[ControlThread] server_cx = " << com_data->server_cx << endl << flush;
	  // cout << "[ControlThread] server_cy = " << com_data->server_cy << endl << flush;
	  // cout << "[ControlThread] screen width  = " << settings->getDesktop()->getCurrentScreen().width() << endl;
	  // cout << "[ControlThread] screen height = " << settings->getDesktop()->getCurrentScreen().height() << endl;
	  int server_width = com_data->server_cx * settings->getDesktopScalingFactor();
	  int server_height = com_data->server_cy * settings->getDesktopScalingFactor();
	  int client_width = settings->getDesktop()->getCurrentScreen().width();
	  int client_height = settings->getDesktop()->getCurrentScreen().height();

	  client_width *= settings->getAutoresizeDesktopScalingFactor();
	  client_height *= settings->getAutoresizeDesktopScalingFactor();

	  if (server_width >= client_width ||
		  server_height >= client_height){
		// auto scale down
		qreal sf_width = client_width/(qreal)server_width;
		qreal sf_height = client_height/(qreal)server_height;
		qreal sf = sf_width < sf_height ? sf_width : sf_height;
		settings->setDesktopScalingFactor(sf);

		// adjust window position
		QSize newWindowSize = QSize(server_width*sf, server_height*sf);
		emit adjustWindow(newWindowSize);
	  }
	}

	// checked password
	doneCheckPassword = true;

	if (!settings->getOnBrynhildr2Support()){
	  // same as Brynhildr (<= 1.1.5)
	  com_data->server_version = SERVER_VERSION_BRYNHILDR;
	}

	// check server version
	checkServerVersion();
  }

  // check monitor no
  if (monitorCount != com_data->monitor_count){
	monitorCount = com_data->monitor_count;
	//	cout << "[ControlThread] monitor_count=" << (int)monitorCount << endl << flush;
	settings->setMonitorCount(monitorCount);
	emit refreshMenu();
  }

  // counter up
  if (counter_control < NEXT_CONNECT_COUNT){
	counter_control++;
  }

  // check result (mode)
  checkMode();

  // save frame no of server
  frameNoOfServer = (uchar)com_data->frame_no;
  //cout << "[" << name << "] frameNoOfServer : " << frameNoOfServer << endl << flush;

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT ControlThread::transmitBuffer()
{
  // receive clilpboard/file
  if (!receiveFromServer())
	return TRANSMIT_RESTART;

  return TRANSMIT_SUCCEEDED;
}

// connected
void ControlThread::connectedToServer()
{
  // reset counter
  counter_control = 0;

  // save frame no of server
  frameNoOfServer = 0;

  // save frame no of client
  frameNoOfClient = 0;

  // mouse position
  prevPos.x = -1;
  prevPos.y = -1;

  // shift/alt/control status
  keydownSHIFT	= KEYDOWN_OFF;
  keydownALT	= KEYDOWN_OFF;
  keydownCONTROL= KEYDOWN_OFF;

#if 0 // defined(QTB_DEV_TOUCHPANEL)
  // set initial mouse position
  mouseBuffer->setPos(400, 400);
  desktopPanel->setMousePos(400, 400);
#endif // defined(QTB_DEV_TOUCHPANEL)

  NetThread::connectedToServer();
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
// send header
long ControlThread::sendHeader(const char *buf, long size)
{
  return NetThread::sendHeader(sock_control, buf, size);
}

// send data
long ControlThread::sendData(const char *buf, long size)
{
  return NetThread::sendData(sock_control, buf, size);
}

// receive data
long ControlThread::receiveData(char *buf, long size)
{
  return NetThread::receiveData(sock_control, buf, size);
}

// setup header
void ControlThread::setupHeader()
{
  // initialize header(com_data)
  initHeader();

  // for control
  if (settings->getOnControl()){
	//-------------------------------------------
	// 1) Mouse Control
	//-------------------------------------------
	setMouseControl();

	//-------------------------------------------
	// 2) Keyboard Control
	//-------------------------------------------
	setKeyboardControl();

	//-------------------------------------------
	// 3) GamePad Control
	//-------------------------------------------
	if (settings->getOnGamePadSupport()){
	  setGamePadControl();
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
#endif // for DEBUG
}

// init protocol header
void ControlThread::initHeader()
{
  // zero clear
  memset(com_data, 0, sizeof(COM_DATA));

  // set protocol version string
  memcpy(com_data->ver,
		 PROTOCOL_VERSION_STRING, PROTOCOL_VERSION_STRING_LENGTH);

  initHeaderForCommon();

  initHeaderForControl();

#if QTB_TEST
  initHeaderForGraphics_test();
#else // QTB_TEST
  initHeaderForGraphics();
#endif // QTB_TEST

  initHeaderForSound();
}

// initialize protocol header for common
void ControlThread::initHeaderForCommon()
{
 // common
  com_data->data_type	= DATA_TYPE_DATA;
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
  com_data->thread		= THREAD_CONTROL;
  com_data->mode		= settings->getPublicModeVersion();
  com_data->monitor_no	= settings->getMonitorNo();
  if (serverVersion == SERVER_VERSION_BRYNHILDR2){
	com_data->mouse_cursor = settings->getOnDisplayMouseCursor() ? MOUSE_CURSOR_ON : MOUSE_CURSOR_OFF;
	if (settings->getOnShowSoftwareKeyboard() || settings->getOnShowSoftwareButton())
	  com_data->mouse_cursor = MOUSE_CURSOR_ON;
  }
}

// initialize protocol header for control
void ControlThread::initHeaderForControl()
{
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
}

#if defined(QTB_DEV_DESKTOP)
// initialize protocol header for graphics for desktop
void ControlThread::initHeaderForGraphics()
{
  // for graphics
  // video quality
  com_data->video_quality	= settings->getVideoQuality();
  // max fps
  if (onMaxfps){
	if (settings->getOnGraphics()){
	  com_data->max_fps		= (char)settings->getFrameRate();
	}
	else {
	  com_data->max_fps		= (char)1;
	}
  }
  // frame_no
  com_data->frame_no = (char)frameNoOfClient;

  // zoom
  if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER &&
	  settings->getDesktopScalingFactor() < 1.0){
	// scale down on server : zoom > 1.0
	com_data->zoom			= (ZOOM)settings->getDesktopScalingFactorForZoom();
  }
  else {
	com_data->zoom			= (ZOOM)1.0;
  }

  // client scroll
  // com_data->client_scroll_x	= (POS)settings->getDesktopOffsetX();
  // com_data->client_scroll_y	= (POS)settings->getDesktopOffsetY();
  // com_data->scroll = 1; // enable scroll (public mode 7)

  // image size
  com_data->image_cx		= (SIZE)settings->getDesktopWidth();
  com_data->image_cy		= (SIZE)settings->getDesktopHeight();
  // com_data->image_cx		= 640; // (SIZE)settings->getDesktopWidth();
  // com_data->image_cy		= 400; // (SIZE)settings->getDesktopHeight();
  // com_data->zoom = 1.0;
  // com_data->client_scroll_x	= -10;
  // com_data->client_scroll_y	= -20;
  // com_data->scroll = 1; // enable scroll (public mode 7)

#if QTB_DESKTOP_COMPRESS_MODE
  // desktop compress mode
  if (settings->getDesktopCompressMode() > 1){
	ZOOM compressZoom = settings->getDesktopCompressMode();
	com_data->zoom *= compressZoom;
	// com_data->image_cx = com_data->image_cx/compressZoom;
	// com_data->image_cy = com_data->image_cy/compressZoom;
  }
#endif // QTB_DESKTOP_COMPRESS_MODE

  //cout << "(image_cx, image_cy) = (" << com_data->image_cx << ", " << com_data->image_cy << ")" << endl << flush;
}
#else // defined(QTB_DEV_DESKTOP)
// initialize protocol header for graphics for touchpanel
void ControlThread::initHeaderForGraphics()
{
  // for graphics
  // video quality
  com_data->video_quality	= settings->getVideoQuality();
  // max fps
  if (onMaxfps){
	if (settings->getOnGraphics()){
	  com_data->max_fps		= (char)settings->getFrameRate();
	}
	else {
	  com_data->max_fps		= (char)1;
	}
  }
  // frame_no
  com_data->frame_no = (char)frameNoOfClient;

  // image size and zoom
  SIZE imageWidth = settings->getDesktopWidth();
  SIZE imageHeight = settings->getDesktopHeight();

  if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER &&
	  settings->getDesktopScalingFactor() < 1.0){
	// scale down on server : zoom > 1.0
	com_data->zoom = (ZOOM)settings->getDesktopScalingFactorForZoom();
	// com_data->client_scroll_x	= 0;
	// com_data->client_scroll_y	= 0;
  }
  else {
	// zoom = 1.0 (original size)
	com_data->zoom			= (ZOOM)1.0;

	// offset x,y
	POS client_scroll_x	= (POS)settings->getDesktopOffsetX();
	POS client_scroll_y	= (POS)settings->getDesktopOffsetY();
	com_data->client_scroll_x	= client_scroll_x;
	com_data->client_scroll_y	= client_scroll_y;
	com_data->scroll = 1; // enable scroll (public mode 7)

    // set initial image size
	imageWidth -= client_scroll_x;
	imageHeight -= client_scroll_y;

	// setup image size
	if (settings->getOnWindowSizeFixed()){
	  qreal scalingFactor = settings->getDesktopScalingFactor();
	  QSize windowSize = desktopPanel->getWindowSize();
	  //QSize windowSize = QSize(640, 400); // for TEST

	  if (windowSize.isValid()){
		if (windowSize.width() < imageWidth*scalingFactor ||
			windowSize.height() < imageHeight*scalingFactor){
		  imageWidth = windowSize.width()/scalingFactor;
		  imageHeight = windowSize.height()/scalingFactor;
		}

		//cout << "scaling Factor = " << scalingFactor << endl;
		//cout << "windowSize = (" << windowSize.width() << ", " << windowSize.height() << ")" << endl;
		//cout << "(scroll_x, scroll_y) = (" << client_scroll_x << ", " << client_scroll_y << ")" << endl;
		//cout << "(imageWidth, imageHeight) = (" << imageWidth << ", " << imageHeight << ")" << endl;
		//cout << flush;
	  }
	}
  }

  com_data->image_cx = imageWidth;
  com_data->image_cy = imageHeight;

  //cout << "(imageWidth, imageHeight) = (" << imageWidth << ", " << imageHeight << ")" << endl << flush;
}
#endif // defined(QTB_DEV_DESKTOP)

// initialize protocol header for graphics
void ControlThread::initHeaderForGraphics_test()
{
  // for graphics
  // video quality
  com_data->video_quality	= settings->getVideoQuality();
  // max fps
  if (onMaxfps){
	if (settings->getOnGraphics()){
	  com_data->max_fps		= (char)settings->getFrameRate();
	}
	else {
	  com_data->max_fps		= (char)1;
	}
  }
  // zoom
  com_data->zoom = (ZOOM)1.0;
  settings->setDesktopScalingFactor(1/com_data->zoom);

  com_data->scroll = 1;

  com_data->client_scroll_x	= 0;
  com_data->client_scroll_y	= 0;
  com_data->image_cx = 1280;
  com_data->image_cy = 800;
}

// initialize protocol header for sound
void ControlThread::initHeaderForSound()
{
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

// set mouse control
void ControlThread::setMouseControl()
{
  // filedrop
  if (mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_FILEDROP) == MOUSE_BUTTON_UP){
	com_data->mouse_left = MOUSE_BUTTON_UP;
	com_data->filedrop = FILEDROP_ON;
  }
  else { // setup mouse buttons
	com_data->mouse_right = mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_RIGHT);
	com_data->mouse_left = mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_LEFT);
#if QTB_EXTRA_BUTTON_SUPPORT
	com_data->mouse_middle = mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_MIDDLE);
	com_data->mouse_x1 = mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_BACK);
	com_data->mouse_x2 = mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_FORWARD);
#endif // QTB_EXTRA_BUTTON_SUPPORT
	// setup mouse wheel
	com_data->mouse_wheel = mouseBuffer->getWheel();
  }

  // setup mouse position
  MOUSE_POS pos = mouseBuffer->getPos();
  // if mouse cursor is moved.
  QSize windowSize = desktopPanel->getSize();
  QSize desktopImageSize = settings->getDesktopImageSize();
  if (!(windowSize.isValid() && desktopImageSize.isValid())){
	// Nothing to do
	com_data->mouse_move = MOUSE_MOVE_OFF;
  }
  else if (prevPos.x != pos.x || prevPos.y != pos.y || settings->getOnHoldMouseControl()){
	// set information
	com_data->mouse_move = MOUSE_MOVE_ON;
	if (settings->getDesktopScalingType() == DESKTOPSCALING_TYPE_ON_SERVER){
	  qreal scalingFactor = settings->getDesktopScalingFactorForZoom();
	  if (scalingFactor > 1.0){
		com_data->mouse_x = pos.x * desktopImageSize.width()/windowSize.width() * scalingFactor;
		com_data->mouse_y = pos.y * desktopImageSize.height()/windowSize.height() * scalingFactor;
	  }
	  else {
		com_data->mouse_x = pos.x * desktopImageSize.width()/windowSize.width();
		com_data->mouse_y = pos.y * desktopImageSize.height()/windowSize.height();
	  }
	}
	else {
	  com_data->mouse_x = pos.x * desktopImageSize.width()/windowSize.width();
	  com_data->mouse_y = pos.y * desktopImageSize.height()/windowSize.height();
	}
	// set offset
	com_data->mouse_x += settings->getDesktopOffsetX();
	com_data->mouse_y += settings->getDesktopOffsetY();
#if QTB_DESKTOP_COMPRESS_MODE
	// desktop compress mode
	int desktopCompressMode = settings->getDesktopCompressMode();
	if (desktopCompressMode > 1){
	  com_data->mouse_x *= desktopCompressMode;
	  com_data->mouse_y *= desktopCompressMode;
	}
#endif // QTB_DESKTOP_COMPRESS_MODE

	//cout << "com_data->mouse_x = " << com_data->mouse_x << endl;
	//cout << "com_data->mouse_y = " << com_data->mouse_y << endl;

	// move to xxxx
	if (mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_MOVE_TOPSIDE) == MOUSE_BUTTON_UP){
	  com_data->mouse_move = MOUSE_MOVE_ON;
	  com_data->mouse_y = 0;
	}
	else if (mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_MOVE_BOTTOMSIDE) == MOUSE_BUTTON_UP){
	  com_data->mouse_move = MOUSE_MOVE_ON;
	  com_data->mouse_y = settings->getDesktopHeight()-1;
	}
	else if (mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_MOVE_LEFTSIDE) == MOUSE_BUTTON_UP){
	  com_data->mouse_move = MOUSE_MOVE_ON;
	  com_data->mouse_x = 0;
	}
	else if (mouseBuffer->getButton(MouseBuffer::MOUSE_BUTTON_MOVE_RIGHTSIDE) == MOUSE_BUTTON_UP){
	  com_data->mouse_move = MOUSE_MOVE_ON;
	  com_data->mouse_x = settings->getDesktopWidth()-1;
	}

	// save pos
	prevPos = pos;
  }
}

// set keyboard control
void ControlThread::setKeyboardControl()
{
  KeyInfo *keyInfo = keyBuffer->get();
  if (keyInfo != 0){
	// check shift/alt/control status
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

// set gamepad control
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

// check server version
void ControlThread::checkServerVersion()
{
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

// check mode
void ControlThread::checkMode()
{
  // save current mode
  if (currentMode == 0){
	currentMode = settings->getPublicModeVersion();
  }
  else if (currentMode != settings->getPublicModeVersion()){
	currentMode = settings->getPublicModeVersion();
	// change mouse cursor
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
}

#if QTB_RECORDER
// recorder function
void ControlThread::recordAndReplayHeader()
{
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
}
#endif // QTB_RECORDER

// send to server
bool ControlThread::sendToServer()
{
  bool result = true;

 if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// cliboard
	if (com_data->data_type == DATA_TYPE_CLIPBOARD &&
		settings->getOnTransferClipboardSupport()){
	  // send clipboard
	  result = sendClipboard();
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 settings->getOnTransferFileSupport()){
	  // send file
	  keyBuffer->setEnabled(false); // disabled keyboard
	  mouseBuffer->setEnabled(false); // disabled mouse
	  result = sendFile();
	  keyBuffer->setEnabled(true);
	  mouseBuffer->setEnabled(true);
	}
  }

  return result;
}

// receive from server
bool ControlThread::receiveFromServer()
{
  bool result = true;

  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION6){
	// cliboard
	if (com_data->data_type == DATA_TYPE_CLIPBOARD &&
		settings->getOnTransferClipboardSupport()){
	  // receive clipboard
	  result = receiveClipboard();
	}
	// file
	else if (com_data->data_type == DATA_TYPE_FILE &&
			 settings->getOnTransferFileSupport()){
	  // receive file
	  keyBuffer->setEnabled(false); // disabled keyboard
	  mouseBuffer->setEnabled(false); // disabled mouse
	  result = receiveFile();
	  keyBuffer->setEnabled(true);
	  mouseBuffer->setEnabled(true);
	}
	// mouse cursor image
	else if (com_data->data_type == DATA_TYPE_DATA){
	  // receive mouse cursor image
	  result = receiveMouseCursorImage();
	}
  }

  return result;
}

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
  sentDataSize = sendData(localBuffer, stringSize + 16);
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
	receivedDataSize = receiveData(buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	clipboardSize -= receivedDataSize;
  }
  if (clipboardSize > 0){
	receivedDataSize = receiveData(buffer, clipboardSize);
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
  char filename[QTB_FILENAME_IMAGE_SIZE+1] = {0};
  char fileTimeStamp[QTB_TIMESTAMP_IMAGE_SIZE+1] = {0};
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
  strncpy(filename, qPrintable(fileInfo.fileName()), QTB_FILENAME_IMAGE_SIZE);
  sentDataSize = sendData(filename, QTB_FILENAME_IMAGE_SIZE);

  // send time stamp
#if QT_VERSION >= 0x050a00 // Qt 5.10.0
  qint64 CreationTime = ntfs->toFILETIME(fileInfo.birthTime()); // UTC
#else // QT_VERSION >= 0x050a00
  qint64 CreationTime = ntfs->toFILETIME(fileInfo.created()); // UTC
#endif // QT_VERSION >= 0x050a00
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
  sentDataSize = sendData(fileTimeStamp, QTB_TIMESTAMP_IMAGE_SIZE);

  // send file image
  fstream file;
  file.open(qPrintable(fileName), ios::in | ios::binary);
  if (file.is_open()){
	while(fileSize > QTB_CONTROL_LOCAL_BUFFER_SIZE){
	  // read to buffer
	  file.read(buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
	  // send to server
	  sentDataSize = sendData(buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
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
	  sentDataSize = sendData(buffer, fileSize + paddingSize);
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
  char filename[QTB_FILENAME_IMAGE_SIZE+1];
  char fileTimeStamp[QTB_TIMESTAMP_IMAGE_SIZE+1]; // dummy read
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
  receivedDataSize = receiveData(filename, QTB_FILENAME_IMAGE_SIZE);

  // get file time stamp
  receivedDataSize = receiveData(fileTimeStamp, QTB_TIMESTAMP_IMAGE_SIZE);

  // get file image
  fstream file;
  QString localFilename = settings->getOutputPath() + filename;
  file.open(qPrintable(localFilename), ios::out | ios::binary);
  if (file.is_open()){
	while(fileSize > QTB_CONTROL_LOCAL_BUFFER_SIZE){
	  receivedDataSize = receiveData(buffer, QTB_CONTROL_LOCAL_BUFFER_SIZE);
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
	  receivedDataSize = receiveData(buffer, fileSize);
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

// receive mouse cursor image
bool ControlThread::receiveMouseCursorImage()
{
  qint64 receivedDataSize = 0;

  // get AND Mask Cursor
  receivedDataSize = receiveData((char*)andMaskImage, QTB_ICON_IMAGE_SIZE);
  if (receivedDataSize != QTB_ICON_IMAGE_SIZE){
	return false;
  }
#if SAVE_MOUSE_CURSOR_IMAGE_BINARY // for TEST
  else {
	fstream file;
	file.open("jpg/andMaskImage.bin", ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write((char*)andMaskImage, receivedDataSize);
	  file.close();
	}
	QImage image(andMaskImage, QTB_ICON_WIDTH, QTB_ICON_HEIGHT, QImage::Format_RGBA8888);
	image = image.mirrored(false, true);
	image.save("jpg/andMaskImage.bmp", "BMP");
  }
#endif // for TEST

  // get XOR Mask Cursor
  receivedDataSize = receiveData((char*)xorMaskImage, QTB_ICON_IMAGE_SIZE);
  if (receivedDataSize != QTB_ICON_IMAGE_SIZE){
	return false;
  }
#if SAVE_MOUSE_CURSOR_IMAGE_BINARY // for TEST
  else {
	fstream file;
	file.open("jpg/xorMaskImage.bin", ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write((char*)xorMaskImage, receivedDataSize);
	  file.close();
	}
	QImage image(xorMaskImage, QTB_ICON_WIDTH, QTB_ICON_HEIGHT, QImage::Format_RGBA8888);
	image = image.mirrored(false, true);
	image.save("jpg/xorMaskImage.bmp", "BMP");
  }
#endif // for TEST

  if (!settings->getOnDisplayMouseCursor()){
	// check null cursor image
	bool nullFlag = true;
	for(int i = 0; i < QTB_ICON_IMAGE_SIZE; i++){
	  if (andMaskImage[i] != 0 || xorMaskImage[i] != 0){
		nullFlag = false;
		break;
	  }
	}
	if (nullFlag){ // found null cursor image
	  // change mouse cursor
	  const QCursor nullCursor;
	  emit changeMouseCursor(nullCursor);

	  return true;
	}

	// BGRA -> RGBA
	for(int i = 0; i < QTB_ICON_IMAGE_SIZE; i += 4){
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
  if (isColorMouseCursorImage(xorMaskImage, QTB_ICON_IMAGE_SIZE)){
	newCursor = createColorMouseCursor(xorMaskImage, andMaskImage);
  }
  else {
	newCursor = createMonochromeMouseCursor(xorMaskImage, andMaskImage);
  }
  // change mouse cursor
  emit changeMouseCursor(newCursor);
}

// check color mouse cursor image
bool ControlThread::isColorMouseCursorImage(uchar *image, int size)
{
  for(int i = 0; i < size; i += 4){
	if ((image[i]   != 0 && image[i]   != 0xFF) || // R
		(image[i+1] != 0 && image[i+1] != 0xFF) || // G
		(image[i+2] != 0 && image[i+2] != 0xFF) || // B
		 image[i+3] != 0 ){ // A
	  //cout << "Found Color Mouse Cursor!" << endl << flush;
	  return true;
	}
  }

  //cout << "Found Monochrome Mouse Cursor!" << endl << flush;
  return false;
}

// create color mouse cursor
QCursor ControlThread::createColorMouseCursor(uchar *image, uchar *mask)
{
#if HEXDUMP_MOUSE_CURSOR_IMAGE_BINARY // for TEST
  cout << hex << uppercase << setfill('0');
  cout << endl << "======= image - R" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+0];
  }
  cout << endl << "======= image - G" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+1];
  }
  cout << endl << "======= image - B" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+2];
  }
  cout << endl << "======= image - A" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+3];
  }
  cout << endl << "======= mask - R" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+0];
  }
  cout << endl << "======= mask - G" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+1];
  }
  cout << endl << "======= mask - B" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+2];
  }
  cout << endl << "======= mask - A" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+3];
  }
  cout << endl << flush;
#endif // HEXDUMP_MOUSE_CURSOR_IMAGE_BINARY // for TEST

  bool flag24bit = true;
  // check A
  for(int i = 0; i < QTB_ICON_IMAGE_SIZE; i += 4){
	if (image[i+3] != 0){
	  flag24bit = false;
	  break;
	}
  }
  // set mask to A (RGBA)
  if (flag24bit){
	for(int i = 0; i < QTB_ICON_IMAGE_SIZE; i += 4){
	  image[i+3] = ~mask[i];
	}
  }

  // Cursor Image
  QImage cursorImage(image, QTB_ICON_WIDTH, QTB_ICON_HEIGHT, QImage::Format_RGBA8888);
  cursorImage = cursorImage.mirrored(false, true);
#if SAVE_MOUSE_CURSOR_IMAGE
  cursorImage.save("jpg/cursorImage.bmp", "BMP");
#endif // SAVE_MOUSE_CURSOR_IMAGE
  QPixmap cursor = QPixmap::fromImage(cursorImage, Qt::NoFormatConversion);

  // change mouse cursor image
#if SAVE_MOUSE_CURSOR_IMAGE // for TEST
  cursor.save("jpg/ZCursor_pixmap.bmp", "BMP");
#endif // SAVE_MOUSE_CURSOR_IMAGE // for TEST
  int hotX = (int)com_data->cursor_hotspot_x;
  int hotY = (int)com_data->cursor_hotspot_y;
  return QCursor(cursor, hotX, hotY);
}

// create monochrome mouse cursor
QCursor ControlThread::createMonochromeMouseCursor(uchar *image, uchar *mask)
{
#if HEXDUMP_MOUSE_CURSOR_IMAGE_BINARY // for TEST
  cout << hex << uppercase << setfill('0');
  cout << endl << "======= image - R" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+0];
  }
  cout << endl << "======= image - G" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+1];
  }
  cout << endl << "======= image - B" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+2];
  }
  cout << endl << "======= image - A" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)image[i+3];
  }
  cout << endl << "======= mask - R" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+0];
  }
  cout << endl << "======= mask - G" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+1];
  }
  cout << endl << "======= mask - B" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+2];
  }
  cout << endl << "======= mask - A" << endl;
  for(int i = 0, counter = 0; i < QTB_ICON_IMAGE_SIZE; i += 4, counter++){
	if (counter > 0 && (counter % QTB_ICON_WIDTH == 0))
	  cout << endl;
	cout << setw(2) << (int)mask[i+3];
  }
  cout << endl << flush;
#endif // HEXDUMP_MOUSE_CURSOR_IMAGE_BINARY // for TEST

  uchar bitmapImage[QTB_ICON_SIZE*3];
  uchar maskImage[QTB_ICON_SIZE*3];
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
  for (int i = 0; i < QTB_ICON_IMAGE_SIZE; i++){
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

#if !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP) // for XOR'd all platforms
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
#endif // !defined(Q_OS_WIN) && defined(QTB_DEV_DESKTOP)

  QImage bitmapQImage(bitmapImage, QTB_ICON_WIDTH, QTB_ICON_HEIGHT, QImage::Format_RGB888);
  bitmapQImage = bitmapQImage.mirrored(false, true);
  QBitmap bitmap = QBitmap::fromImage(bitmapQImage);
  QImage maskQImage(maskImage, QTB_ICON_WIDTH, QTB_ICON_HEIGHT, QImage::Format_RGB888);
  maskQImage = maskQImage.mirrored(false, true);
  QBitmap maskBitmap = QBitmap::fromImage(maskQImage);
  int hotX = (int)com_data->cursor_hotspot_x;
  int hotY = (int)com_data->cursor_hotspot_y;
#if SAVE_MOUSE_CURSOR_IMAGE // for TEST
  bitmap.save("jpg/ZCursor_bitmap.bmp", "BMP");
  maskBitmap.save("jpg/ZCursor_mask.bmp", "BMP");
#endif // SAVE_MOUSE_CURSOR_IMAGE // for TEST

  return QCursor(bitmap, maskBitmap, hotX, hotY);
}

} // end of namespace qtbrynhildr
