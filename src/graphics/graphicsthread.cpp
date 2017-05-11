// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cmath>
#include <fstream>
#include <iostream>

// Qt Header

// Local Header
#include "graphicsthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
GraphicsThread::GraphicsThread(Settings *settings, MainWindow *mainWindow)
  :
  NetThread("GraphicsThread", settings, mainWindow),
  image(0),
  frameCounter(0),
  totalFrameCounter(0),
  onClearDesktop(false),
  buffer(0)
{
  outputLog = false; // for DEBUG

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

  // create image
  image = new QImage();
}

// destructor
GraphicsThread::~GraphicsThread()
{
  // delete objects
  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }

  // image (QImage)
  if (image != 0){
	delete image;
	image = 0;
  }
}

// get frame rate
double GraphicsThread::getFrameRate()
{
  if (!settings->getOnGraphics()) return 0.0;

  QDateTime currentTime = QDateTime::currentDateTime();
  double fps = 0.0;

  if (!previousFrameTime.isNull()){
	qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousFrameTime.toMSecsSinceEpoch();
	if (diffMSeconds != 0){
	  fps = frameCounter / ((double)diffMSeconds/1000);
	}
  }
  previousFrameTime = currentTime;
  totalFrameCounter += frameCounter;
  frameCounter = 0;
  return fps;
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// connect to server
CONNECT_RESULT GraphicsThread::connectToServer()
{
  if (sock_graphics != INVALID_SOCKET)
	return CONNECT_SUCCEEDED;

  if (counter_control < 5){
	return CONNECT_WAITED_COUNT;
  }

  // connect
  sock_graphics = socketToServer();
  if (sock_graphics == INVALID_SOCKET){
	return CONNECT_FAILED;
  }

  // create objects

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
  // frame control
  if (QTB_DESKTOP_FRAME_CONTROL){
	unsigned long frameDrawTime = settings->getFrameDrawTime();
	if (frameDrawTime == 0){
	  // check frameDrawTime
	  settings->setFrameInterval(0);
	  QDateTime currentTime = QDateTime::currentDateTime();
	  static QDateTime previousTime;
	  static qint64 minDrawTime = 1000; // 1 second
	  static int counter = 0;
	  if (!previousTime.isNull()){
		qint64 drawTime = currentTime.toMSecsSinceEpoch() - previousTime.toMSecsSinceEpoch();
		//		cout << "draw Time = " << drawTime << endl << flush;
		if (counter > QTB_GRAPHICS_SAMPLE_FRAME){
		  //		  cout << "minimum Draw Time = " << minDrawTime << endl << flush;
		  settings->setFrameDrawTime(minDrawTime*1000);
		  settings->setFrameRate(settings->getFrameRate());
		  // for next time
		  previousTime = QDateTime();
		  minDrawTime = 1000; // 1 second
		  counter = 0;
		}
		else {
		  if (drawTime < minDrawTime)
			minDrawTime = drawTime;
		  counter++;
		}
	  }
	  previousTime = currentTime;
	}

	unsigned long frameInterval = settings->getOnGraphics() ? settings->getFrameInterval() : 1000*200;
	if (frameInterval != 0){
	  // sleep frameInterval micro seconds
	  usleep(frameInterval);
	}
  }

  // receive header
  long dataSize;
  dataSize = receiveData(sock_graphics, (char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

  // counter up
  if (counter_graphics < 5){
	counter_graphics++;
  }

#if 0 // for TEST
  static bool run1Time = true;
  if (run1Time && counter_graphics == 5){
	run1Time = false;
	printHeader();
	saveHeader("receivedHeader.dat");
  }
#endif // for TEST

  // check received video_mode
  if (com_data->video_mode != VIDEO_MODE_MJPEG){
	return PROCESS_VIDEO_MODE_ERROR;
  }

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT GraphicsThread::transmitBuffer()
{
  // receivedDataSize (= JPEG File Size)
  long receivedDataSize = com_data->data_size;

  // check
  if (receivedDataSize <= 0){
	// Nothing to do
	return TRANSMIT_DATASIZE_ERROR;
  }
  if (receivedDataSize > QTB_GRAPHICS_LOCAL_BUFFER_SIZE){
	if (outputLog){
	  cout << "[" << name << "] receivedDataSize = " << receivedDataSize << endl << flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive JPEG File Data
  receivedDataSize = receiveData(sock_graphics, buffer, receivedDataSize);
  // size check
  if (receivedDataSize <= 0){
	// Nothing to do
	return TRANSMIT_NETWORK_ERROR;
  }

  // received 1 frame
  frameCounter++;

  // buffer[]         : JPEG File
  // receivedDataSize : Size of JPEG File

  // for DEBUG : save a JPEG File
  if (settings->getOutputGraphicsDataToFile()){
  //  if (true){
	fstream file;
	char filename[QTB_MAXPATHLEN+1];
	int result;
	result = snprintf(filename, QTB_MAXPATHLEN, "jpg/%s_%06d.jpg", QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX, frameCounter);
	if (result > 0 && result <= QTB_MAXPATHLEN){
	  file.open(filename, ios::out | ios::binary | ios::trunc);
	  if (file.is_open()){
		file.write(buffer, receivedDataSize);
		file.close();
	  }
	}
	else {
	  // snprintf() error
	  if (settings->getOutputLog()){
		cout << "[GraphicsThread] snprintf() error!" << endl << flush;
	  }
	}
  }

  // draw a JPEG File
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

	// load a JPEG data to desktop
	bool desktopLoadResult;
	desktopLoadResult = image->loadFromData((const uchar *)buffer,
											(uint)receivedDataSize,
											"JPEG");
	if (desktopLoadResult){
	  // GOOD
	  // update desktop
	  emit desktopChanged(*image);
	}
	else {
	  // NG
	  if (outputLog){
		cout << "[GraphicsThread:debug] Received Image Data Size = " << receivedDataSize << endl << flush;
	  }
	  // Failed to load JPEG image
	  return TRANSMIT_FAILED_IMAGE_DRAW;
	}
  }
  else {
	// clear desktop only at once
	if (!onClearDesktop){
	  onClearDesktop = true;
	  emit desktopClear();
	}
  }

  return TRANSMIT_SUCCEEDED;
}

// connected
void GraphicsThread::connectedToServer()
{
  // reset counter
  counter_graphics = 0;

  // reset frame counter
  frameCounter = 0;

  // reset previous frame time to Null
  previousFrameTime = QDateTime();

  NetThread::connectedToServer();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void GraphicsThread::shutdownConnection()
{
  // reset previous frame time to Null
  previousFrameTime = QDateTime();

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

  NetThread::shutdownConnection();
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

} // end of namespace qtbrynhildr
