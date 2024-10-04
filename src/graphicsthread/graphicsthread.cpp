// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

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
#else // defined(__ARM_NEON__)
#include "decoder_vp8_neon.h"
#endif // defined(__ARM_NEON__)
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
  ,onDrawing(true)
   //,image(new QImage)
#if QTB_SIMD_SUPPORT
  ,hasSIMDInstruction(false)
#endif // QTB_SIMD_SUPPORT
  ,buffer(nullptr)
  ,decoderMode56(nullptr)
  ,decoderMode7(nullptr)
  ,decoder(nullptr)
  ,video_mode(-1)
#if QTB_BENCHMARK
  ,initialBenchmarkPhaseCounter(20)
  ,benchmarkPhaseCounter(0)
#endif // QTB_BENCHMARK
{
  //outputLog = true; // for DEBUG

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
  // local buffer
  if (buffer != nullptr){
	delete [] buffer;
	buffer = nullptr;
  }

  // decoders
  // mode 5,6
  if (decoderMode56 != nullptr){
	delete decoderMode56;
	decoderMode56 = nullptr;
  }
  // mode 7
  QMapIterator<QString, Decoder*> i(decoderMode7Map);
  while (i.hasNext()){
	i.next();
	Decoder *decoder = (Decoder*)i.value();
	delete decoder;
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
  if (sock_graphics == TIMEOUT_SOCKET){
	return CONNECT_FAILED_TIMEOUT;
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
	std::cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << std::endl << std::flush; // error
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
	  std::cout << "[" << name << "] dataSize = " << dataSize << std::endl << std::flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive data for image
  long receivedDataSize = receiveData(buffer, dataSize);

#if 0 // for TEST
  std::cout << "[" << name << "] image_cx = " << (int)com_data->image_cx << std::endl;
  std::cout << "[" << name << "] image_cy = " << (int)com_data->image_cy << std::endl;
  std::cout << "[" << name << "] client_scroll_x = " << (int)com_data->client_scroll_x << std::endl;
  std::cout << "[" << name << "] client_scroll_y = " << (int)com_data->client_scroll_y << std::endl;
  std::cout << "[" << name << "] server_cx = " << (int)com_data->server_cx << std::endl;
  std::cout << "[" << name << "] server_cy = " << (int)com_data->server_cy << std::endl;
  std::cout << "[" << name << "] frame no = " << (int)com_data->frame_no << std::endl;
  std::cout << "[" << name << "] data_size = " << (int)com_data->data_size << std::endl;
  std::cout << "[" << name << "] receivedDataSize = " << receivedDataSize << std::endl << std::flush;
#endif // for TEST

  // size check
  if (receivedDataSize != dataSize){
	// error
	return TRANSMIT_NETWORK_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got data");
#endif // TEST_THREAD

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
	std::fstream file;

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

  // draw 1 frame
  drawDesktopImage(buffer, (int)receivedDataSize, com_data->video_mode);

  return TRANSMIT_SUCCEEDED;
}

// connected
void GraphicsThread::connectedToServer()
{
  // reset counter
  counter_graphics = 0;

  // reset frame counter
  frameCounter.reset();

  // set decoder for MODE7 (VP8)
  decoderMode7 = decoderMode7Map.value(settings->getSIMDOperationTypeName());
#if 0 // for TEST
  if (decoderMode7 != 0){
	std::cout << "decoder : " << decoderMode7->name() << std::endl << std::flush;
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
							(int)receivedDataSize,
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
	emit outputLogMessage(PHASE_GRAPHICS, std::move(str));
  }

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
	// adjust frame time
	//adjustFrame();

#if QTB_GRAY_SCALE_MODE
	// set gray scale
	static bool previousOnGrayScale = false;
	if (previousOnGrayScale != settings->getOnMonochromeMode()){
	  decoder->setOnGrayScale(settings->getOnMonochromeMode());
	  previousOnGrayScale = settings->getOnMonochromeMode();
	}
#endif // QTB_GRAY_SCALE_MODE

	// check frame skip
	if (doSkipFrame(com_data->frame_no)){
	  // skip this frame
	}
	else {
	  // draw this frame
	  QImage *image = decoder->getDesktopImage(settings->getConvertThreadCount());

	  if (settings->getOnViewerMode()){
		// copy area in viewer mode
		qreal scalingFactor = settings->getDesktopScalingFactorForZoom();
		if (scalingFactor > 1.0){
		  if (settings->getDesktopOffsetX() != 0)
			*image = image->copy(settings->getDesktopOffsetX(),
								 settings->getDesktopOffsetY(),
								 settings->getDesktopImageWidth(),
								 settings->getDesktopImageHeight());
		}
	  }

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

		// rescale image
		rescaleDesktopImage(image);

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

	// save frame_no of client
	frameNoOfClient = com_data->frame_no;
  }
}

#if defined(QTB_DEV_DESKTOP)
// rescale image for desktop
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
#if !defined(QTB_DEV_TOUCHPANEL)
		*image = image->scaled(currentSize, Qt::KeepAspectRatio, settings->getDesktopScaringQuality());
		//*image = image->scaled(currentSize, Qt::KeepAspectRatio, Qt::FastTransformation);
		//*image = image->scaled(currentSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif // !defined(QTB_DEV_TOUCHPANEL)
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
#elif defined(QTB_DEV_TOUCHPANEL)
// rescale image for touchpanel
#if 1 // for TEST
void GraphicsThread::rescaleDesktopImage(QImage *image)
{
#if 0 // for TEST
  qreal sfz = settings->getDesktopScalingFactorForZoom();
  if (sfz >= 1.0){
	// original size
  }
  else if (sfz < 1.0){
	// fit to screen
	*image = image->scaledToWidth(settings->getDesktopWidth()); // for TEST
  }
#else // 0 // for TEST
  qreal sf = settings->getDesktopScalingFactor();
  if (sf > 1.0){
	*image = image->scaled(settings->getDesktopWidth() * sf,
						   settings->getDesktopHeight()* sf); // for TEST
  }
#endif // 0 // for TEST
}
#else // 1 // for TEST
void GraphicsThread::rescaleDesktopImage(QImage *image)
{
  // Nothing to do
}
#endif // 1 // for TEST
#endif // defined(QTB_DEV_TOUCHPANEL)

// get desktop scaling factor
qreal GraphicsThread::getDesktopScalingFactor(QSize size)
{
  Q_UNUSED(size);

  qreal scalingFactor = settings->getDesktopScalingFactor();

  if (scalingFactor != 1.0){
	unsigned long maxImageDataSize = settings->getDesktop()->getMaxImageDataSize();
	if (maxImageDataSize == 0){
	  if (settings->getOutputLog()){
		std::cout << "[DesktopPanel] scaled... maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
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
			std::cout << "[DesktopPanel] Can't scale... imageDataSize    = " << imageDataSize << std::endl;
			std::cout << "[DesktopPanel] Can't scale... maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
		  }
		  // scale down
		  scalingFactor -= unitFactor;
		  if (scalingFactor < unitFactor){
			return unitFactor;
		  }
		}
	  } // end of while
	}
#elif  defined(Q_OS_MACOS)
	else {
	  // Darwin
	  // Internal Error: Unknown State
	  ABORT();
	}
#endif
  }

  return scalingFactor;
}

// adjust frame
// void GraphicsThread::adjustFrame()
// {
// }

// check skip frame
bool GraphicsThread::doSkipFrame(char frame_no)
{
  //std::cout << "[" << name << "] frame_no : " << (int)(uchar)frame_no << std::endl << std::flush;

  int currentFrameNo = (uchar)frame_no;

  if (currentFrameNo % 2 == 0)
	// NOT skip frame
	return false;

  // check skip frame
  int frameNo = frameNoOfServer;
  //std::cout << "frameNo : " << frameNo << std::endl << std::flush;
  //std::cout << "currentFrameNo : " << currentFrameNo << std::endl << std::flush;
  if (frameNo < currentFrameNo){
	 frameNo += 255;
  }

  int delay = frameNo - currentFrameNo;
  int threshold = settings->getFrameRate() >> 3;

  if (delay > threshold){
	// need skip frame
	//std::cout << "doSkipFrame! delay =  " << delay << std::endl << std::flush;
	return true;
  }
  else {
	// no need skip frame
	return false;
  }
}

} // end of namespace qtbrynhildr
