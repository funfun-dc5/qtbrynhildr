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

// for TEST
#define TEST_THREAD			0

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
GraphicsThread::GraphicsThread(Settings *settings)
  :NetThread("GraphicsThread", settings)
  ,graphicsBuffer(0)
  ,frameCounter(0)
  ,totalFrameCounter(0)
  ,buffer(0)
{
  outputLog = false; // for DEBUG

  // graphics buffer
  graphicsBuffer = new GraphicsBuffer(1024*1024); // for TEST (1MB)

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];
}

// destructor
GraphicsThread::~GraphicsThread()
{
  // delete objects
  // graphics buffer
  if (graphicsBuffer != 0){
	delete graphicsBuffer;
	graphicsBuffer = 0;
  }

  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }
}

// get frame rate
double GraphicsThread::getFrameRate()
{
#if 0 // for TEST
  if (!settings->getOnGraphics()) return 0.0;
#endif // 0 // for TEST

  qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  double fps = 0.0;

  if (previousGetFrameRateTime != 0){
	qint64 diffMSeconds = currentTime - previousGetFrameRateTime;
	if (diffMSeconds != 0){
	  fps = frameCounter / ((double)diffMSeconds/1000);
	  //cout << "frameCounter = " << frameCounter << endl;
	  //cout << "diffMSeconds = " << diffMSeconds << endl << flush;
	}
  }
  previousGetFrameRateTime = currentTime;
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

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

#if TEST_THREAD
  qint64 startTime;
#endif // TEST_THREAD

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
#if TEST_THREAD
  startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
  {
	static qint64 previousTime = 0;
	qint64 duration = 0;
	if (previousTime != 0){
	  duration = startTime - previousTime;
	}
	previousTime = startTime;
	cout << "================================   " << duration << endl;
  }
#endif // TEST_THREAD

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

#if TEST_THREAD
  {
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startTime;
	cout << "[" << name << "] got header      : " << pastTime << endl;
  }
#endif // TEST_THREAD

  // counter up
  if (counter_graphics < 5){
	counter_graphics++;
  }

  // check received video_mode
#if QTB_PUBLIC_MODE7_SUPPORT
  if (com_data->video_mode != VIDEO_MODE_MJPEG &&
	  com_data->video_mode != VIDEO_MODE_COMPRESS ){
	return PROCESS_VIDEO_MODE_ERROR;
  }
#else // QTB_PUBLIC_MODE7_SUPPORT
  if (com_data->video_mode != VIDEO_MODE_MJPEG){
	return PROCESS_VIDEO_MODE_ERROR;
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT GraphicsThread::transmitBuffer()
{
  // receivedDataSize for image
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

  // receive data for image
  receivedDataSize = receiveData(sock_graphics, buffer, receivedDataSize);
  // size check
  if (receivedDataSize <= 0){
	// error
	return TRANSMIT_NETWORK_ERROR;
  }

#if TEST_THREAD
  {
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startTime;
	cout << "[" << name << "] got data        : " << pastTime
		 << " (size = " << receivedDataSize << ")" << endl;
  }
#endif // TEST_THREAD

#if 0 // for TEST
  cout << "[" << name << "] frame no = " << (int)com_data->frame_no << endl;
  cout << "[" << name << "] receivedDataSize = " << receivedDataSize << endl << flush;
#endif

  // == VIDEO_MODE_MJPEG ==
  // buffer[]         : JPEG File
  // receivedDataSize : Size of JPEG File
  // == VIDEO_MODE_COMPRESS ==
  // buffer[]         : VP8 Data
  // receivedDataSize : Size of VP8 Data

  // for DEBUG : save received data
  if (settings->getOutputGraphicsDataToFile()){
  //  if (true){
	fstream file;
	char filename[QTB_MAXPATHLEN+1];
	int result;
	if (com_data->video_mode == VIDEO_MODE_MJPEG){
	  result = snprintf(filename, QTB_MAXPATHLEN, "jpg/%s_%06d.jpg", QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX, frameCounter);
	}
	else { // binary
	  result = snprintf(filename, QTB_MAXPATHLEN, "jpg/%s_%06d.bin", QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX, frameCounter);
	}
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

#if 0 // for TEST
  // put data into graphics buffer
  if (settings->getOnGraphics()){
	// put into graphics buffer
	int putSize = graphicsBuffer->putFrame(buffer, receivedDataSize);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into graphics buffer
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}
	// get from graphics buffer
	int len = graphicsBuffer->getFrame(buffer);
	if (len != receivedDataSize){
	  cout << "Graphics Buffer : getFrame() failed!" << endl << flush;
	}
  }
#endif // 1 // for TEST

  // received 1 frame
  frameCounter++;

#if 0 // for TEST
  // draw a desktop image
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

	bool desktopLoadResult = false;
	// ------------------------------------------------------------------------------
	// Motion JPEG (Mode 5/6)
	// ------------------------------------------------------------------------------
	if (com_data->video_mode == VIDEO_MODE_MJPEG){
	  // load a JPEG data to desktop
	  desktopLoadResult = image->loadFromData((const uchar *)buffer,
											  (uint)receivedDataSize,
											  "JPEG");
	}

#if QTB_PUBLIC_MODE7_SUPPORT
	// ------------------------------------------------------------------------------
	// VP8 (Mode 7)
	// ------------------------------------------------------------------------------
	else if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	  // VP8
#if USE_PPM_LOADER_FOR_VP8
#if QTB_SIMD_SUPPORT
	  int rgbImageSize;
	  if (hasSIMDInstruction && settings->getOnSIMDOperationSupport()){
		// make rgb image by using SIMD instruction
		rgbImageSize = makeRGBImage_SIMD();
	  }
	  else {
		rgbImageSize = makeRGBImage();
	  }
#else // QTB_SIMD_SUPPORT
	  int rgbImageSize = makeRGBImage();
#endif // QTB_SIMD_SUPPORT
	  if (rgbImageSize != 0){
		// load a PPM data to desktop
		desktopLoadResult = image->loadFromData((const uchar *)ppm,
												(uint)rgbImageSize + PPM_HEADER_SIZE_MAX,
												"PPM");
	  }
	  else {
		if (image->isNull()){
		  delete image;
		  image = new QImage(width, height, IMAGE_FORMAT);
		  image->fill(QTB_DESKTOP_BACKGROUND_COLOR);
		}
	  }
#else // USE_PPM_LOADER_FOR_VP8
#if QTB_SIMD_SUPPORT
	  int rgbImageSize;
	  if (hasSIMDInstruction && settings->getOnSIMDOperationSupport()){
		// make rgb image by using SIMD instruction
		//cout << "SIMD!" << endl << flush;
		rgbImageSize = makeRGBImage_SIMD();
	  }
	  else {
		//cout << "no SIMD!" << endl << flush;
		rgbImageSize = makeRGBImage();
	  }
#else // QTB_SIMD_SUPPORT
	  int rgbImageSize = makeRGBImage();
#endif // QTB_SIMD_SUPPORT
#if 0 // for getting test data
	  {
		static int frameNo = 1;
		if (frameNo == 40){ // sampling at 40th frame
		  fstream file;
		  // save yuv420
		  file.open("yuv420_1280x800.dat", ios::out | ios::binary | ios::trunc);
		  if (file.is_open()){
			file.write((char*)yuv420, width * height + width * height / 2);
			file.close();
		  }
		  // save yuv420prev
		  if (yuv420 == yuv1){
			file.open("yuv420prev_1280x800.dat", ios::out | ios::binary | ios::trunc);
			if (file.is_open()){
			  file.write((char*)yuv2, width * height + width * height / 2);
			  file.close();
			}
		  }
		  else {
			file.open("yuv420prev_1280x800.dat", ios::out | ios::binary | ios::trunc);
			if (file.is_open()){
			  file.write((char*)yuv1, width * height + width * height / 2);
			  file.close();
			}
		  }
		  // save rgb24
		  file.open("rgb24_1280x800.dat", ios::out | ios::binary | ios::trunc);
		  if (file.is_open()){
			file.write((char*)rgb, width * height * IMAGE_FORMAT_SIZE);
			file.close();
		  }
		}
		frameNo++;
	  }
#endif // for getting test data
	  if (rgbImageSize != 0){
		// create QImage from RGB
		delete image;
		//		cout << "rgbImageSize = " << rgbImageSize << endl << flush;
		image = new QImage(rgb, width, height, IMAGE_FORMAT);
	  }
	  else {
		if (image->isNull()){
		  delete image;
		  image = new QImage(width, height, IMAGE_FORMAT);
		  image->fill(QTB_DESKTOP_BACKGROUND_COLOR);
		}
	  }

	  desktopLoadResult = true;
#endif // USE_PPM_LOADER_FOR_VP8
	}
	else {
	  // ------------------------------------------------------------------------------
	  // unknown VIDEO_MODE
	  // ------------------------------------------------------------------------------
	  // desktopLoadResult = false;
	}
#endif // QTB_PUBLIC_MODE7_SUPPORT

#if TEST_FRAME_CONTROL
  if (QTB_DESKTOP_FRAMERATE_CONTROL){
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startDrawFrameTime;
	cout << "[" << name << "] emit draw       : " << pastTime << endl;
  }
#endif // TEST_FRAME_CONTROL

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
#endif // 0 // for TEST

  return TRANSMIT_SUCCEEDED;
}

// connected
void GraphicsThread::connectedToServer()
{
  // reset counter
  counter_graphics = 0;

  // reset frame counter
  frameCounter = 0;

  // reset previous frame time
  previousGetFrameRateTime = 0;

  NetThread::connectedToServer();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void GraphicsThread::shutdownConnection()
{
  // reset previous frame time to Null
  previousGetFrameRateTime = 0;

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

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
