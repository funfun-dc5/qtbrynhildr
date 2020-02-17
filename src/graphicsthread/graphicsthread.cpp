// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

// Qt Header

// Local Header
#include "decoder_jpeg.h"
#include "decoder_vp8_cpp.h"
#if !defined(__ARM_NEON__)
#include "decoder_vp8_sse.h"
#if defined(__AVX2__)
#include "decoder_vp8_avx2.h"
#endif // defined(__AVX2__)
#else // !defined(__ARM_NEON__)
#include "decoder_vp8_neon.h"
#endif // !defined(__ARM_NEON__)
#include "graphicsthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"
#if !QTB_TEST_CODE
#include "util/cpuinfo.h"
#include "yuv2rgb/yuv2rgb.h"
#endif // !QTB_TEST_CODE

// for TEST
#define TEST_THREAD		0

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
  ,onDrawing(true)
#if !QTB_TEST_CODE
  ,image(new QImage)
  ,onClearDesktop(false)
#if QTB_SIMD_SUPPORT
  ,hasSIMDInstruction(false)
#endif // QTB_SIMD_SUPPORT
#endif // !QTB_TEST_CODE
  ,buffer(0)
 #if QTB_BENCHMARK
  ,initialBenchmarkPhaseCounter(20)
  ,benchmarkPhaseCounter(0)
#endif // QTB_BENCHMARK
  ,decoderMode56(0)
  ,decoderMode7(0)
  ,decoderMode7SIMD(0)
  ,decoder(0)
  ,video_mode(-1)
{
  //outputLog = true; // for DEBUG

#if QTB_TEST_CODE
  // graphics buffer
  graphicsBuffer = new GraphicsBuffer(settings->getGraphicsBufferSize());
#endif // QTB_TEST_CODE

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

#if !QTB_TEST_CODE
  initVPX();

#if QTB_SIMD_SUPPORT
#if 0 // for TEST

#if !defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::SSE42() || CPUInfo::AVX2();
#else // !defined(__ARM_NEON__)
  hasSIMDInstruction = CPUInfo::NEON();
#endif // !defined(__ARM_NEON__)

#else // 0 // for TEST

  // set decoders
  decoderMode56 = new DecoderJPEG(image);
  decoderMode7 = new DecoderVP8CPP(image);
#if !defined(__ARM_NEON__)
#if defined(__AVX2__)
  if (CPUInfo::AVX2()){
	decoderMode7SIMD = new DecoderVP8AVX2(image);
	hasSIMDInstruction = true;
  }
  else if (CPUInfo::SSE42()){
	decoderMode7SIMD = new DecoderVP8SSE(image);
	hasSIMDInstruction = true;
  }
#else // defined(__AVX2__)
  if (CPUInfo::SSE42()){
	decoderMode7SIMD = new DecoderVP8SSE(image);
	hasSIMDInstruction = true;
  }
#endif // defined(__AVX2__)
#else // !defined(__ARM_NEON__)
  if (CPUInfo::NEON()){
	decoderMode7SIMD = new DecoderVP8NEON(image);
	hasSIMDInstruction = true;
  }
#endif // !defined(__ARM_NEON__)

#endif // 0 // for TEST
#endif // QTB_SIMD_SUPPORT
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

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// connect to server
CONNECT_RESULT GraphicsThread::connectToServer()
{
  if (sock_graphics != INVALID_SOCKET)
	return CONNECT_SUCCEEDED;

  if (counter_control < NEXT_CONNECT_COUNT){
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

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
  // set start time of drawing a frame
#if TEST_THREAD
  startTimeInfo();
#endif // TEST_THREAD

  // receive header
  long dataSize;
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

  // counter up
  if (counter_graphics < NEXT_CONNECT_COUNT){
	counter_graphics++;
  }

  // check received video_mode
  if (com_data->video_mode != VIDEO_MODE_MJPEG &&
	  com_data->video_mode != VIDEO_MODE_COMPRESS ){
	return PROCESS_VIDEO_MODE_ERROR;
  }

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT GraphicsThread::transmitBuffer()
{
  // data size for image
  long dataSize = com_data->data_size;

#if QTB_BENCHMARK
  // set initial counter for benchmark
  benchmarkPhaseCounter = initialBenchmarkPhaseCounter;
#endif // QTB_BENCHMARK

  // check
  if (dataSize <= 0){
	// Nothing to do
	return TRANSMIT_DATASIZE_ERROR;
  }
  if (dataSize > QTB_GRAPHICS_LOCAL_BUFFER_SIZE){
	if (outputLog){
	  cout << "[" << name << "] dataSize = " << dataSize << endl << flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive data for image
  long receivedDataSize = receiveData(buffer, dataSize);
  // size check
  if (receivedDataSize != dataSize){
	// error
	return TRANSMIT_NETWORK_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got data");
#endif // TEST_THREAD

#if 0 // for TEST
  cout << "[" << name << "] frame no = " << (int)com_data->frame_no << endl;
  cout << "[" << name << "] receivedDataSize = " << receivedDataSize << endl << flush;
#endif

  // == VIDEO_MODE_MJPEG ==
  // buffer[]         : JPEG File Image
  // receivedDataSize : Size of JPEG File Image

  // == VIDEO_MODE_COMPRESS ==
  // buffer[]         : VP8 Data
  // receivedDataSize : Size of VP8 Data

  // for TEST : save received data
  if (settings->getOutputGraphicsDataToFile()){
	outputReceivedData(receivedDataSize);
  }

  // received 1 frame
  frameCounter.countUp();

#if 0 // for TEST
  {
	decoder->preprocess(buffer, receivedDataSize);
	QImage *image;
	image = decoder->getDesktopImage(1);
	if (image == nullptr)
	  cout << "DECODER: " << decoder->name() << endl << flush;
  }
#endif // 0 // for TEST

#if QTB_BENCHMARK
  // check benchmark phase counter
  benchmarkPhaseCounter--;
  if (benchmarkPhaseCounter < 0){
	return TRANSMIT_SUCCEEDED;
  }
#endif // QTB_BENCHMARK

#if 0 // for TEST
  {
	fstream file;

	file.open("test.vp8", ios::out | ios::binary | ios::app);
	if (file.is_open()){
	  int size = (int)receivedDataSize;
	  file.write((char *)&size, sizeof(int));
	  file.write(buffer, receivedDataSize);
	  file.close();
	}
  }
#endif // for TEST

  // for network test
  if (!onDrawing)
	return TRANSMIT_SUCCEEDED;

#if !QTB_TEST_CODE

#if 1 // for TEST

  // check mode
  if (com_data->video_mode == video_mode){ // no change mode
	// MODE 7 (VP8)
	if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	  if (hasSIMDInstruction){
		if (decoder == decoderMode7){
		  if(settings->getOnSIMDOperationSupport())
			decoder = decoderMode7SIMD;
		}
		else { // decoder == decoderMode7SIMD
		  if(!settings->getOnSIMDOperationSupport())
			decoder = decoderMode7;
		}
	  }
	}
  }
  else { // change mode
	// change decoder
	// MODE 5/6 (MJPEG)
	if (com_data->video_mode == VIDEO_MODE_MJPEG){
	  decoder = decoderMode56;
	}
	// MODE 7 (VP8)
	else if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	  if (hasSIMDInstruction && settings->getOnSIMDOperationSupport()){
		decoder = decoderMode7SIMD;
	  }
	  else {
		decoder = decoderMode7;
	  }
	}
  }

  // pre-process
  decoder->preprocess(buffer, receivedDataSize);

  // draw graphics
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

	if (frameControler.adjust()){
	  QImage *image = decoder->getDesktopImage(settings->getConvertThreadCount());
	  if (image != nullptr){
		//  image->save("jpg/desktop.jpg", "jpg", 75);
		emit drawDesktop(*image);
	  }
	}
  }
  else {
	// clear desktop only at once
	if (!onClearDesktop){
	  onClearDesktop = true;
	  emit clearDesktop();
	}
  }

#else // 1 // for TEST

  // decode VP8
  if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	decodeVPX((uchar*)buffer, receivedDataSize);
  }

#if TEST_THREAD
  printTimeInfo("decoded VP8");
#endif // TEST_THREAD

#if QTB_BENCHMARK
  // check benchmark phase counter
  benchmarkPhaseCounter--;
  if (benchmarkPhaseCounter < 0){
	return TRANSMIT_SUCCEEDED;
  }
#endif // QTB_BENCHMARK

  // draw graphics
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

	if (frameControler.adjust()){
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
  printTimeInfo("emit draw");
#endif // TEST_THREAD

#if 0 // NG
  // frame rate control
  if (QTB_DESKTOP_FRAMERATE_CONTROL){
	qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qint64 pastTime = threadSleepTime + currentTime - startTime;
	qint64 interval = settings->getFrameInterval();
	//cout << "interval = " << interval << ", pastTime = " << pastTime << endl << flush;

	if (pastTime < interval){
	  qint64 sleepTime = interval - pastTime;
	  //cout << "sleepTime = " << sleepTime << endl << flush;
	  QThread::msleep(sleepTime);
	}
	else {
	  //cout << "sleepTime = 0" << endl << flush;
	}
  }
#endif // 0 // NG

#if TEST_THREAD
  printTimeInfo("frame controled");
#endif // TEST_THREAD

#endif // 1 // for TEST

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
  frameCounter.reset();

  NetThread::connectedToServer();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void GraphicsThread::shutdownConnection()
{
  // reset frame counter
  frameCounter.reset();

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
// receive data
long GraphicsThread::receiveData(char *buf, long size)
{
  return NetThread::receiveData(sock_graphics, buf, size);
}

#if !QTB_TEST_CODE

#if 0 // for TEST

// draw graphics
void GraphicsThread::draw_Graphics(int size)
{
  bool result = false;

  // MODE 5/6 (MJPEG)
  if (com_data->video_mode == VIDEO_MODE_MJPEG){
	// load a JPEG data
	result = draw_Graphics_MJPEG(size);
  }
  // MODE 7 (VP8)
  else if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	// load a RGB32 data from YUV image
	result = draw_Graphics_COMPRESS(size);
  }

#if QTB_BENCHMARK
  // check benchmark phase counter
  benchmarkPhaseCounter--;
  if (benchmarkPhaseCounter < 0){
	return;
  }
#endif // QTB_BENCHMARK

  // draw desktop
  if (result){
	//  image->save("jpg/desktop.jpg", "jpg", 75);
	emit drawDesktop(*image);
  }
}

// draw graphics MJPEG
inline bool GraphicsThread::draw_Graphics_MJPEG(int size)
{
  bool result = false;

  // load a JPEG data to QImage
  result = image->loadFromData((const uchar *)buffer,
							   (uint)size,
							   "JPEG");
  if (!result){
	// internal error (illigal JPEG file image)
	cout << "internal error (illigal JPEG file image)" << endl << flush;
	ABORT();
  }

  return result;
}

// draw graphics COMPRESS
inline bool GraphicsThread::draw_Graphics_COMPRESS(int size)
{
  Q_UNUSED(size);

  bool result = false;

  // make RGB image
  int rgbImageSize;

#if QTB_SIMD_SUPPORT
  if (hasSIMDInstruction && settings->getOnSIMDOperationSupport()){
	rgbImageSize = makeRGBImage(convertYUVtoRGB_SIMD, settings->getConvertThreadCount());
  }
  else {
	rgbImageSize = makeRGBImage(convertYUVtoRGB, settings->getConvertThreadCount());
  }
#else // QTB_SIMD_SUPPORT
  rgbImageSize = makeRGBImage(convertYUVtoRGB, settings->getConvertThreadCount());
#endif // QTB_SIMD_SUPPORT

  //  cout << "rgbImageSize = " << rgbImageSize << endl << flush;

#if QTB_BENCHMARK
  // check benchmark phase counter
  benchmarkPhaseCounter--;
  if (benchmarkPhaseCounter < 0){
	return false;
  }
#endif // QTB_BENCHMARK

  if (rgbImageSize != 0){
#if 0 // for TEST
	{
	  static bool flag = true;
	  fstream file;

	  if (flag){
		file.open("test.bmp", ios::out | ios::binary);
		if (file.is_open()){
		  int size = (int)rgbImageSize + 64;
		  file.write((const char*)qtbrynhildr::bmp, size);
		  file.close();
		}
		//flag = false;
	  }
	}
#endif // for TEST
#if QTB_LOAD_BITMAP
	// load a BMP data to QImage
	result = image->loadFromData((const uchar *)qtbrynhildr::bmp,
								 (uint)rgbImageSize + 64,
								 "BMP");
#else // QTB_LOAD_BITMAP
	// create QImage
	if (image != 0){
	  delete image;
	}
	image = new QImage(qtbrynhildr::rgb, qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
	result = true;
#endif // QTB_LOAD_BITMAP
  }
  else {
	if (image->isNull()){
	  delete image;
	  image = new QImage(qtbrynhildr::width, qtbrynhildr::height, IMAGE_FORMAT);
	  image->fill(QTB_DESKTOP_BACKGROUND_COLOR);
	}
  }

  return result;
}

#endif // 0 // for TEST

#endif // !QTB_TEST_CODE

// output received data
void GraphicsThread::outputReceivedData(long receivedDataSize)
{
  decoder->outputDataToFile(buffer, receivedDataSize, frameCounter.getFrameCounter());
}

} // end of namespace qtbrynhildr
