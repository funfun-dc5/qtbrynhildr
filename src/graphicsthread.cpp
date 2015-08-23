// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// graphicsthread.cpp

// Common Header
#include "common.h"

// System Header
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
  // com_data
  if (com_data != 0){
	delete com_data;
	com_data = 0;
  }

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
double GraphicsThread::getFrameRate() const
{
  static QDateTime previsouTime;
  static unsigned int previousFrameCount;
  QDateTime currentTime = QDateTime::currentDateTime();
  unsigned int currentFrameCount = getFrameCounter();
  double fps = 0.0;

  if (!previsouTime.isNull()){
	qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previsouTime.toMSecsSinceEpoch();
	qint64 diffFrameCount = currentFrameCount - previousFrameCount;
	fps = diffFrameCount / ((double)diffMSeconds/1000);
	//	cout << "diffMSeconds   = " << diffMSeconds << endl << flush;
	//	cout << "diffFrameCount = " << diffFrameCount << endl << flush;
  }
  previsouTime = currentTime;
  previousFrameCount = currentFrameCount;
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
  // header
  if (com_data == 0){
	com_data = new COM_DATA;
  }

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
  // frame control
  if (QTB_DESKTOP_FRAME_CONTROL){
	unsigned long frameInterval = settings->getFrameInterval();
	if (frameInterval != 0){
	  // sleep frameInterval micro seconds
	  usleep(frameInterval);
	}
  }

  // receive header
  if (receiveData(sock_graphics, (char *)com_data, sizeof(COM_DATA)) != sizeof(COM_DATA)){
	// error
	cout << "[" << name << "]" << " received data size != sizeof(COM_DATA)" << endl << flush; // error
	return PROCESS_NETWORK_ERROR;
  }

  // counter up
  if (counter_graphics < 5){
	counter_graphics++;
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
	cout << "[" << name << "] receivedDataSize = " << receivedDataSize << endl << flush; // error
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
	snprintf(filename, QTB_MAXPATHLEN, "jpg/%s_%06d.jpg", QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX, frameCounter);
	file.open(filename, ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write(buffer, receivedDataSize);
	  file.close();
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

  // frame counter
  frameCounter = 0;
}

// shutdown connection
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
void GraphicsThread::shutdownConnection()
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
#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX)

} // end of namespace qtbrynhildr
