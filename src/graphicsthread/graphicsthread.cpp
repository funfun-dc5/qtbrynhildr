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

#if QTB_SIMD_SUPPORT
#if !defined(__ARM_NEON__)
#if defined(__AVX2__)
#include "decoder_vp8_avx2.h"
#endif // defined(__AVX2__)
#if defined(__AVX__)
#include "decoder_vp8_avx.h"
#endif // defined(__AVX__)
#if defined(__SSE4_2__)
#include "decoder_vp8_sse.h"
#endif // defined(__SSE4_2__)
#else // !defined(__ARM_NEON__)
#include "decoder_vp8_neon.h"
#endif // !defined(__ARM_NEON__)
#endif // QTB_SIMD_SUPPORT

#include "graphicsthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"
#include "util/cpuinfo.h"

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
  ,graphicsBufferSize(0)
  ,graphicsBuffer(0)
#endif // QTB_TEST_CODE
  ,onDrawing(true)
   //,image(new QImage)
  ,onClearDesktop(false)
#if QTB_SIMD_SUPPORT
  ,hasSIMDInstruction(false)
#endif // QTB_SIMD_SUPPORT
  ,buffer(0)
  ,decoderMode56(0)
  ,decoderMode7(0)
  ,decoder(0)
  ,video_mode(-1)
#if QTB_BENCHMARK
  ,initialBenchmarkPhaseCounter(20)
  ,benchmarkPhaseCounter(0)
#endif // QTB_BENCHMARK
{
  //outputLog = true; // for DEBUG

#if QTB_TEST_CODE
  // graphics buffer
  graphicsBufferSize = settings->getGraphicsBufferSize();
  graphicsBuffer = new GraphicsBuffer(graphicsBufferSize);
#endif // QTB_TEST_CODE

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

  // set decoders
  // for MODE5/6
  decoderMode56 = new DecoderJPEG();

  // for MODE7
#if QTB_SIMD_SUPPORT
#if !defined(__ARM_NEON__)
#if defined(__AVX2__)
  if (CPUInfo::AVX2()){
	decoderMode7 = new DecoderVP8AVX2();
	decoderMode7Map.insert(decoderMode7->name(), decoderMode7);
	decoderMode7NameList.insert(0, decoderMode7->name());
	hasSIMDInstruction = true;
  }
#endif // defined(__AVX2__)
#if defined(__AVX__)
  if (CPUInfo::AVX()){
	decoderMode7 = new DecoderVP8AVX();
	decoderMode7Map.insert(decoderMode7->name(), decoderMode7);
	decoderMode7NameList.insert(0, decoderMode7->name());
	hasSIMDInstruction = true;
  }
#endif // defined(__AVX__)
#if defined(__SSE4_2__)
  if (CPUInfo::SSE42()){
	decoderMode7 = new DecoderVP8SSE();
	decoderMode7Map.insert(decoderMode7->name(), decoderMode7);
	decoderMode7NameList.insert(0, decoderMode7->name());
	hasSIMDInstruction = true;
  }
#endif // defined(__SSE4_2__)
#else // !defined(__ARM_NEON__)
  if (CPUInfo::NEON()){
	decoderMode7 = new DecoderVP8NEON();
	decoderMode7Map.insert(decoderMode7->name(), decoderMode7);
	decoderMode7NameList.insert(0, decoderMode7->name());
	hasSIMDInstruction = true;
  }
#endif // !defined(__ARM_NEON__)
#endif // QTB_SIMD_SUPPORT

  decoderMode7 = new DecoderVP8CPP();
  decoderMode7Map.insert(decoderMode7->name(), decoderMode7);
  decoderMode7NameList.insert(0, decoderMode7->name());

  //qDebug() << "decoderMode7NameList : " << decoderMode7NameList;
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
	graphicsBufferSize = 0;
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

  // draw 1 frame
  drawDesktopImage(buffer, receivedDataSize, com_data->video_mode);

#else // !QTB_TEST_CODE

  // put data into graphics buffer
  if (settings->getOnGraphics() || com_data->video_mode == VIDEO_MODE_COMPRESS){
	// block
	while (graphicsBuffer->getFrameCount() >= GraphicsBuffer::FRAME_TABLE_NUM ||
		   graphicsBuffer->getSize() + receivedDataSize > graphicsBufferSize){
	  QThread::msleep(5); // 5 milli seconds sleep
	}

	unsigned int rate = settings->getFrameRate();
	int putSize = graphicsBuffer->putFrame(buffer, receivedDataSize, com_data->video_mode, rate);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into graphics buffer
	  cout << "Failed to put into graphics buffer" << endl << flush;
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}

	// get from graphics buffer
	VIDEO_MODE mode;
	int getSize = graphicsBuffer->getFrame(buffer, mode, rate);
	if (getSize != 0){
	  // draw 1 frame
	  drawDesktopImage(buffer, getSize, mode);
	}
	else {
	  cout << "Graphics Buffer is empty!" << endl << flush;
	}
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

  // reset frame controller
  frameController.reset();

  // set decoder for MODE7 (VP8)
  decoderMode7 = decoderMode7Map.value(settings->getSIMDOperationTypeName());
#if 0 // for TEST
  if (decoderMode7 != 0){
	cout << "decoder : " << decoderMode7->name() << endl << flush;
  }
  else {
	qDebug() << "decoderMode7 == 0 for " << settings->getSIMDOperationTypeName();
  }
#endif // 0 // for TEST
  // initialize for yuv, rgb
  initYUV2RGB();

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

// output received data
void GraphicsThread::outputReceivedData(long receivedDataSize)
{
  decoder->outputDataToFile(buffer,
							receivedDataSize,
							frameCounter.getFrameCounter());
}

// draw desktop image
void GraphicsThread::drawDesktopImage(char *buf, int size, VIDEO_MODE mode)
{
  // check mode
  if (mode != video_mode){ // change mode
	// change decoder
	// MODE 5/6 (MJPEG)
	if (mode == VIDEO_MODE_MJPEG){
	  decoder = decoderMode56;
	}
	// MODE 7 (VP8)
	else if (mode == VIDEO_MODE_COMPRESS){
	  decoder = decoderMode7;
	}
	// save current mode
	video_mode = mode;

	QString str = "Decoder : " + QString(decoder->name());
	emit outputLogMessage(PHASE_GRAPHICS, str);
  }

  // record the start time of decode
  frameController.startDecode();

  // pre-process
  decoder->preprocess(buf, size);

#if QTB_BENCHMARK
  // check benchmark phase counter
  benchmarkPhaseCounter--;
  if (benchmarkPhaseCounter < 0){
	return;
  }
#endif // QTB_BENCHMARK

  // draw graphics
  if (settings->getOnGraphics()){
	// clear desktop flag clear
	onClearDesktop = false;

	if (frameController.adjust(com_data->frame_no, settings->getFrameInterval())){
	  QImage *image = decoder->getDesktopImage(settings->getConvertThreadCount());

#if QTB_BENCHMARK
	  // check benchmark phase counter
	  benchmarkPhaseCounter--;
	  if (benchmarkPhaseCounter < 0){
		return;
	  }
#endif // QTB_BENCHMARK

	  if (image != nullptr){
		// save desktop image size
		settings->setDesktopImageSize(image->size());

#if !(defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW)
		// rescale image
		rescaleDesktopImage(image);
#endif // !(defined(QTB_DEV_TOUCHPANEL) || QTB_NEW_DESKTOPWINDOW)

#if QTB_BENCHMARK
		// check benchmark phase counter
		benchmarkPhaseCounter--;
		if (benchmarkPhaseCounter < 0){
		  return;
		}
#endif // QTB_BENCHMARK

		//  image->save("jpg/desktop.jpg", "jpg", 75);
		emit drawDesktop(*image);
	  }
	}
	// record the end time of decode
	frameController.endDecode();
  }
  else {
	// clear desktop only at once
	if (!onClearDesktop){
	  onClearDesktop = true;
	  emit clearDesktop();
	}
  }
}

// rescale image
void GraphicsThread::rescaleDesktopImage(QImage *image)
{
  // cut blank area
  if (QTB_CUT_DESKTOP_BLANK_AREA){
	// for offset
	if (settings->getOnCutDesktopBlankArea()){
	  if (settings->getDesktopOffsetX() != 0 || settings->getDesktopOffsetY() != 0){
		*image = image->copy(0,0,
							 image->size().width()  - settings->getDesktopOffsetX(),
							 image->size().height() - settings->getDesktopOffsetY());
	  }
	}
  }

  // save size info.
  currentSize = image->size();
  bool isSameSize = currentSize == previousSize;

  // capture desktop image (original size)
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	if (settings->getOnDesktopCapture() &&
		!settings->getDesktopCaptureFormat().startsWith(".")){
	  QDateTime now = QDateTime::currentDateTime();
	  QString filename = settings->getOutputPath() +
		QString(QTB_DESKTOP_CAPTURE_FILENAME_PREFIX) +
		now.toString(QTB_DESKTOP_CAPTURE_FILENAME_DATE_FORMAT) +
		"." + settings->getDesktopCaptureFormat();

	  // save to file
	  image->save(filename);

	  // reset desktop capture flag
	  settings->setOnDesktopCapture(false);
	}
  }

  // rescale
  if (QTB_DESKTOP_IMAGE_SCALING){
	switch (settings->getDesktopScalingType()){
	case DESKTOPSCALING_TYPE_ON_SERVER:
	  if (settings->getDesktopScalingFactor() > 1.0){
		// scale up
		currentSize = getSizeForCurrentMode(image->size() * settings->getDesktopScalingFactor());
		*image = image->scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
	  }
	  break;
	case DESKTOPSCALING_TYPE_ON_CLIENT:
	  {
		qreal scalingFactor = getDesktopScalingFactor(currentSize);
		if (!isSameSize){
		  // recalculate scaling factor
		  qreal widthRate = (qreal)previousSize.width()/currentSize.width();
		  qreal heightRate = (qreal)previousSize.height()/currentSize.height();
		  if (settings->getMonitorChangeType() == MONITOR_CHANGE_TYPE_SINGLE_TO_ALL){
			scalingFactor = widthRate < heightRate ? widthRate : heightRate;
		  }
		  else if (settings->getMonitorChangeType() == MONITOR_CHANGE_TYPE_ALL_TO_SINGLE){
			scalingFactor = widthRate > heightRate ? widthRate : heightRate;
		  }
		  // flag clear
		  settings->setMonitorChangeType(MONITOR_CHANGE_TYPE_NONE);
		}
		if (scalingFactor != 1.0){
		  // scale
		  currentSize = getSizeForCurrentMode(currentSize * scalingFactor);
#if !QTB_NEW_DESKTOPWINDOW
		  *image = image->scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
		  //*image = image->scaled(currentSize, Qt::KeepAspectRatio, Qt::FastTransformation);
		  //*image = image->scaled(currentSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif // !QTB_NEW_DESKTOPWINDOW
		}
		// save scaling factor
		if (scalingFactor != settings->getDesktopScalingFactor()){
		  settings->setDesktopScalingFactor(scalingFactor);
		}
	  }
	  break;
	default:
	  // unknown scaling type
	  break;
	}
  }

  // capture desktop image (rescaled size)
  if (QTB_DESKTOP_IMAGE_CAPTURE){
	if (settings->getOnDesktopCapture()){
	  QDateTime now = QDateTime::currentDateTime();
	  QString filename = settings->getOutputPath() +
		QString(QTB_DESKTOP_CAPTURE_FILENAME_PREFIX) +
		now.toString(QTB_DESKTOP_CAPTURE_FILENAME_DATE_FORMAT) +
		settings->getDesktopCaptureFormat();

	  // save to file
	  image->save(filename);

	  // reset desktop capture flag
	  settings->setOnDesktopCapture(false);
	}
  }

  // save size info.
  if (currentSize != previousSize){
	previousSize = currentSize;
  }
}

// get desktop scaling factor
qreal GraphicsThread::getDesktopScalingFactor(QSize size)
{
  Q_UNUSED(size);

  qreal scalingFactor = settings->getDesktopScalingFactor();

  if (scalingFactor != 1.0){
	unsigned long maxImageDataSize = settings->getDesktop()->getMaxImageDataSize();
	if (maxImageDataSize == 0){
	  if (settings->getOutputLog()){
		cout << "[DesktopPanel] scaled... maxImageDataSize = " << maxImageDataSize << endl << flush;
	  }
	  return scalingFactor;
	}
#if defined(Q_OS_WIN)
	else {
	  // Internal Error: Unknown State
	  ABORT();
	}
#elif defined(Q_OS_FREEBSD)
	else {
	  // Yet
	  // Internal Error: Unknown State
	  ABORT();
	}
#elif defined(Q_OS_LINUX)
	else {
	  // for Linux
	  unsigned long pageSizeMask = (unsigned long)getpagesize()-1;
	  qreal unitFactor = 1.0/DesktopScalingDialog::SLIDER_FACTOR;
	  while(true){ // for checking scaling factor
		QSize targetSize = getSizeForCurrentMode(size * scalingFactor);
		unsigned long imageDataSize = targetSize.width() * targetSize.height() * 4;
		imageDataSize = (imageDataSize + pageSizeMask) & ~pageSizeMask;
		if (imageDataSize <= maxImageDataSize){
		  return scalingFactor;
		}
		else {
		  // Can't shmget() in QXcbShmImage::QXcbShmImage() in qxcbbackingstore.cpp
		  if (settings->getOutputLog()){
			cout << "[DesktopPanel] Can't scale... imageDataSize    = " << imageDataSize << endl;
			cout << "[DesktopPanel] Can't scale... maxImageDataSize = " << maxImageDataSize << endl << flush;
		  }
		  // scale down
		  scalingFactor -= unitFactor;
		  if (scalingFactor < unitFactor){
			return unitFactor;
		  }
		}
	  } // end of while
	}
#elif  defined(Q_OS_OSX)
	else {
	  // Darwin
	  // Internal Error: Unknown State
	  ABORT();
	}
#endif
  }

  return scalingFactor;
}

} // end of namespace qtbrynhildr
