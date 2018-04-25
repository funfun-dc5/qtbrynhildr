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
#if !QTB_TEST_CODE
#include "util/cpuinfo.h"
#include "yuv2rgb/yuv2rgb.h"
#endif // !QTB_TEST_CODE

// for TEST
#define TEST_THREAD			0

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
GraphicsThread::GraphicsThread(Settings *settings)
  :NetThread("GraphicsThread", settings)
#if QTB_TEST_CODE
  ,graphicsBuffer(0)
  ,graphicsBufferSize(settings->getGraphicsBufferSize())
#endif // QTB_TEST_CODE
  ,frameCounter(0)
  ,totalFrameCounter(0)
#if !QTB_TEST_CODE
  ,image(new QImage)
  ,onClearDesktop(false)
#if QTB_PUBLIC_MODE7_SUPPORT
#if QTB_SIMD_SUPPORT
  ,hasSIMDInstruction(false)
#endif // QTB_SIMD_SUPPORT
#endif // QTB_PUBLIC_MODE7_SUPPORT
#endif // !QTB_TEST_CODE
  ,buffer(0)
  ,onDrawing(true)
{
  outputLog = false; // for DEBUG

#if QTB_TEST_CODE
  // graphics buffer
  graphicsBuffer = new GraphicsBuffer(settings->getGraphicsBufferSize());
#endif // QTB_TEST_CODE

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

#if !QTB_TEST_CODE
#if QTB_PUBLIC_MODE7_SUPPORT
  initVPX();

#if QTB_SIMD_SUPPORT
#if !defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::SSE41();
#else // !defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::NEON();
#endif // !defined(__ARM_NEON__)
#endif // QTB_SIMD_SUPPORT
#endif // QTB_PUBLIC_MODE7_SUPPORT
#endif // !QTB_TEST_CODE
}

// destructor
GraphicsThread::~GraphicsThread()
{
  // delete objects
#if QTB_TEST_CODE
  // graphics buffer
  if (graphicsBuffer != 0){
	delete graphicsBuffer;
	graphicsBuffer = 0;
  }
#endif // QTB_TEST_CODE

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
	  fps = frameCounter / ((double)diffMSeconds)*1000;
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

#if TEST_THREAD || !QTB_TEST_CODE
static qint64 startTime = 0;
#endif // TEST_THREAD || !QTB_TEST_CODE

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
  // set start time of drawing a frame
  startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

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

  // received 1 frame
  frameCounter++;

#if !QTB_TEST_CODE

#if QTB_PUBLIC_MODE7_SUPPORT
  // decode VP8
  if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	decodeVPX((uchar*)buffer, receivedDataSize);
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

#if TEST_THREAD
  {
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startTime;
	cout << "[" << name << "] decoded VP8     : " << pastTime << endl;
  }
#endif // TEST_THREAD

  // draw graphics
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

    if (onDrawing){
	  draw_Graphics(receivedDataSize);
	}
  }
  else {
	// clear desktop only at once
	if (!onClearDesktop){
	  onClearDesktop = true;
	  emit clearDesktop();
	}
  }

#if TEST_THREAD
  {
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startTime;
	cout << "[" << name << "] emit draw       : " << pastTime << endl;
  }
#endif // TEST_THREAD

  // frame rate control
  if (QTB_DESKTOP_FRAMERATE_CONTROL && onDrawing){
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = QTB_THREAD_SLEEP_TIME + currentTime - startTime;
	qint64 interval = settings->getFrameInterval();

	if (pastTime < interval){
	  qint64 sleepTime = interval - pastTime;
	  sleepTime *= 0.9;
	  //cout << "sleepTime = " << sleepTime << endl << flush;
	  QThread::msleep(sleepTime);
	}
	else {
	  //cout << "sleepTime = 0" << endl << flush;
	}
  }

#if TEST_THREAD
  {
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = currentTime - startTime;
	cout << "[" << name << "] frame controled : " << pastTime << endl;
  }
#endif // TEST_THREAD

#else // !QTB_TEST_CODE

  // put data into graphics buffer
  GraphicsBuffer::FrameType type;
  // put into graphics buffer
  switch(com_data->video_mode){
  case VIDEO_MODE_MJPEG:
	type = GraphicsBuffer::TYPE_JPEG;
	break;
  case VIDEO_MODE_COMPRESS:
	type = GraphicsBuffer::TYPE_VP8;
	break;
  default:
	// internal error
	ABORT();
	break;
  }
  if (settings->getOnGraphics() || type == GraphicsBuffer::TYPE_VP8){
	// block
	while (graphicsBuffer->getFrameCount() >= GraphicsBuffer::FRAME_TABLE_NUM ||
		   graphicsBuffer->getSize() + receivedDataSize > graphicsBufferSize){
	  QThread::msleep(5); // 5 milli seconds sleep
	}

	unsigned int rate = settings->getFrameRate();
	int putSize = graphicsBuffer->putFrame(buffer, receivedDataSize, type, rate);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into graphics buffer
	  cout << "Failed to put into graphics buffer" << endl << flush;
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}
#if 0 // for TEST
	// get from graphics buffer
	int getSize = graphicsBuffer->getFrame(buffer, &type, &rate);
	if (getSize != receivedDataSize){
	  cout << "Graphics Buffer : getFrame() failed!" << endl << flush;
	}
#endif // 0 // for TEST
  }

#endif // !QTB_TEST_CODE

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
#if !QTB_TEST_CODE

// draw graphics
void GraphicsThread::draw_Graphics(int size)
{
  bool result = false;

  // MODE 5/6 (MJPEG)
  if (com_data->video_mode == VIDEO_MODE_MJPEG){
	// load a JPEG data to desktop
	result = image->loadFromData((const uchar *)buffer,
									  (uint)size,
									  "JPEG");
	if (!result){
	  // internal error (illigal JPEG file image)
	  cout << "internal error (illigal JPEG file image)" << endl << flush;
	  ABORT();
	}
  }
#if QTB_PUBLIC_MODE7_SUPPORT
  // MODE 7 (VP8)
  else if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	// make RGB image
	int rgbImageSize;
#if QTB_SIMD_SUPPORT
	if (hasSIMDInstruction && settings->getOnSIMDOperationSupport()){
	  rgbImageSize = makeRGBImage_SIMD(settings->getConvertThreadCount());
	}
	else {
	  rgbImageSize = makeRGBImage(settings->getConvertThreadCount());
	}
#else // QTB_SIMD_SUPPORT
	rgbImageSize = makeRGBImage(settings->getConvertThreadCount());
#endif // QTB_SIMD_SUPPORT
	//  cout << "rgbImageSize = " << rgbImageSize << endl << flush;
	if (rgbImageSize != 0){
#if !USE_PPM_LOADER_FOR_VP8
	  // create QImage
	  if (image != 0){
		delete image;
	  }
	  image = new QImage(qtbrynhildr::rgb, qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
#else // !USE_PPM_LOADER_FOR_VP8
	  // load QImage
	  bool result = image->loadFromData((const uchar *)qtbrynhildr::ppm,
										(uint)rgbImageSize + PPM_HEADER_SIZE_MAX,
										"PPM");
	  if (!result){
		// internal error (illigal PPM file image)
		cout << "internal error (illigal PPM file image)" << endl << flush;
		ABORT();
	  }
#endif // !USE_PPM_LOADER_FOR_VP8
	  result = true;
	}
	else {
	  if (image->isNull()){
		delete image;
		image = new QImage(qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
		image->fill(QTB_DESKTOP_BACKGROUND_COLOR);
	  }
	}
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

  // draw desktop
  if (result){
	//  image->save("jpg/desktop.jpg", "jpg", 75);
	emit drawDesktop(*image);
  }
}

#endif // !QTB_TEST_CODE

} // end of namespace qtbrynhildr
