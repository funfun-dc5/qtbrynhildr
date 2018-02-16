// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cmath>
#if QTB_PUBLIC_MODE7_SUPPORT
#if USE_PPM_LOADER_FOR_VP8
#include <cstdio>
#endif // USE_PPM_LOADER_FOR_VP8
#include <cstring>
#endif // QTB_PUBLIC_MODE7_SUPPORT
#include <fstream>
#include <iostream>

// Qt Header
#if QTB_MULTI_THREAD_CONVERTER
#include <QtConcurrent>
#endif // QTB_MULTI_THREAD_CONVERTER

// Local Header
#include "graphicsthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"

// for TEST
#define TEST_FRAME_CONTROL 0

namespace qtbrynhildr {

#if QTB_MULTI_THREAD_CONVERTER
// for qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
// parameters
int width = 0;
int uvNext = 0;
int rgb24Next = 0;

uchar *yuv420 = 0;
uchar *yuv1 = 0;
uchar *y1topOrg = 0;
uchar *u1topOrg = 0;
uchar *v1topOrg = 0;
uchar *y2topOrg = 0;
uchar *u2topOrg = 0;
uchar *v2topOrg = 0;

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
void convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);
#endif // QTB_MULTI_THREAD_CONVERTER

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
GraphicsThread::GraphicsThread(Settings *settings, DesktopPanel *desktopPanel)
  :
  NetThread("GraphicsThread", settings, desktopPanel),
  image(0),
  frameCounter(0),
  averageDrawFrameTime(0),
  totalFrameCounter(0),
  drawTime(0),
  onClearDesktop(false),
#if QTB_PUBLIC_MODE7_SUPPORT
  width(0),
  height(0),
  yuv1(0),
  yuv2(0),
  yuv420(0),
#if USE_PPM_LOADER_FOR_VP8
  ppm(0),
#endif // USE_PPM_LOADER_FOR_VP8
  rgb24(0),
  doneVpxInit(false),
  hwidth(0),
  y1topOrg(0),
  u1topOrg(0),
  v1topOrg(0),
  y2topOrg(0),
  u2topOrg(0),
  v2topOrg(0),
  size(0),
  uvNext(0),
  rgb24Next(0),
#endif // QTB_PUBLIC_MODE7_SUPPORT
  buffer(0)
{
  outputLog = false; // for DEBUG

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

  // create image
  image = new QImage();

#if QTB_PUBLIC_MODE7_SUPPORT
  // foe vpx
  memset(&c_codec, 0, sizeof(c_codec)); // for coverity scan
#endif // QTB_PUBLIC_MODE7_SUPPORT

  //  cout << "converter source name : " << getConverterSourceName() << endl << flush;
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

#if QTB_PUBLIC_MODE7_SUPPORT
  // buffer for yuv420/rgb24
  if (yuv1 != 0){
	delete [] yuv1;
	yuv1 = 0;
  }
  if (yuv2 != 0){
	delete [] yuv2;
	yuv2 = 0;
  }
  yuv420 = 0;
#if USE_PPM_LOADER_FOR_VP8
  if (ppm != 0){
	delete [] ppm;
	ppm = 0;
	rgb24 = 0;
  }
#else // USE_PPM_LOADER_FOR_VP8
  if (rgb24 != 0){
	delete [] rgb24;
	rgb24 = 0;
  }
#endif // USE_PPM_LOADER_FOR_VP8
#endif // QTB_PUBLIC_MODE7_SUPPORT
}

// get frame rate
double GraphicsThread::getFrameRate()
{
#if 0 // for TEST
  if (!settings->getOnGraphics()) return 0.0;
#endif // 0 // for TEST

  QDateTime currentTime = QDateTime::currentDateTime();
  double fps = 0.0;

  if (!previousGetFrameRateTime.isNull()){
	qint64 diffMSeconds = currentTime.toMSecsSinceEpoch() - previousGetFrameRateTime.toMSecsSinceEpoch();
	if (diffMSeconds != 0){
	  fps = frameCounter / ((double)diffMSeconds/1000);
	  //cout << "frameCounter = " << frameCounter << endl;
	  //cout << "diffMSeconds = " << diffMSeconds << endl << flush;
	  averageDrawFrameTime = (frameCounter != 0) ? diffMSeconds*1000/frameCounter : 0;
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

  // create objects

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT GraphicsThread::processForHeader()
{
  // frame rate control
  if (QTB_DESKTOP_FRAMERATE_CONTROL){
	// record start time of draw frame
	startDrawFrameTime = QDateTime::currentDateTime();
#if 0 // for TEST
	static QDateTime prevTime;
	if (!prevTime.isNull()){
	  qint64 diffMSeconds = startDrawFrameTime.toMSecsSinceEpoch() - prevTime.toMSecsSinceEpoch();
	  if (diffMSeconds != 0){
		//cout << "[" << name << "] processForHeader() : diffMSeconds = " << diffMSeconds << " (ms)" << endl << flush;
	  }
	}
	prevTime = startDrawFrameTime;
#endif
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
	// Nothing to do
	return TRANSMIT_NETWORK_ERROR;
  }

#if 0 // for TEST
  cout << "[" << name << "] frame no = " << (int)com_data->frame_no << endl << flush;
#endif

  // received 1 frame
  frameCounter++;

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

  // save current time for draw time check
  if (QTB_DESKTOP_FRAMERATE_CONTROL){
	if (drawTime == 0){
	  startDrawTime = QDateTime::currentDateTime();
	}
#if 0 // TEST_FRAME_CONTROL
	else {
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 pastTime = currentTime.toMSecsSinceEpoch() - startDrawFrameTime.toMSecsSinceEpoch();
	  cout << "[" << name << "] NETWORK : " << pastTime << " (ms)" << endl;
	}
#endif // TEST_FRAME_CONTROL
  }

#if QTB_PUBLIC_MODE7_SUPPORT
  // decode vp8
  if (com_data->video_mode == VIDEO_MODE_COMPRESS){
	// initialize libvpx
	if (!doneVpxInit){
	  memset(&c_codec, 0, sizeof(c_codec)); // for coverity scan
	  vpx_codec_dec_init(&c_codec, &vpx_codec_vp8_dx_algo, 0, 0);
	  doneVpxInit = true;
	}
	vpx_codec_decode(&c_codec, (uint8_t*)buffer, receivedDataSize, 0, 0);
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

  //return TRANSMIT_SUCCEEDED; // for TEST

  // frame skip check
  if (settings->getOnGraphics()){
	// frame rate control
	if (QTB_DESKTOP_FRAMERATE_CONTROL && drawTime != 0){
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 pastTime = (QTB_THREAD_SLEEP_TIME +
						 currentTime.toMSecsSinceEpoch() - startDrawFrameTime.toMSecsSinceEpoch())*1000;
	  qint64 threshold = averageDrawFrameTime * 2; // settings->getFrameInterval();

#if 0 // for TEST
	  cout << "[" << name << "] pastTime  : " << pastTime  << endl;
	  cout << "[" << name << "] drawTime  : " << drawTime  << endl;
	  cout << "[" << name << "] threshold : " << threshold  << endl;
#endif // 0 // for TEST
	  if (pastTime + drawTime > threshold){
		// drop this frame
#if TEST_FRAME_CONTROL
		cout << "pastTime + drawTime > threshold" << endl;
		cout << "[" << name << "] pastTime  : " << pastTime  << endl;
		cout << "[" << name << "] drawTime  : " << drawTime  << endl;
		cout << "[" << name << "] threshold : " << threshold << endl << flush;
#endif // TEST_FRAME_CONTROL
		return TRANSMIT_SUCCEEDED; // skip this frame
	  }
	}
  }

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
	  int rgb24size = makeRGB24Image();
	  if (rgb24size != 0){
		// load a PPM data to desktop
		desktopLoadResult = image->loadFromData((const uchar *)ppm,
												(uint)rgb24size + PPM_HEADER_SIZE_MAX,
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
	  int rgb24size = makeRGB24Image();
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
			file.write((char*)rgb24, width * height * IMAGE_FORMAT_SIZE);
			file.close();
		  }
		}
		frameNo++;
	  }
#endif // for getting test data
	  if (rgb24size != 0){
		// create QImage from RGB24
		delete image;
		//		cout << "rgb24size = " << rgb24size << endl << flush;
		image = new QImage(rgb24, width, height, IMAGE_FORMAT);
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

  // frame rate control
  if (QTB_DESKTOP_FRAMERATE_CONTROL){
	QDateTime currentTime = QDateTime::currentDateTime();
	qint64 pastTime = (QTB_THREAD_SLEEP_TIME +
					   currentTime.toMSecsSinceEpoch() - startDrawFrameTime.toMSecsSinceEpoch())*1000;
	qint64 interval = settings->getFrameInterval();

	// draw time check
	if (drawTime == 0){
	  static int counter = 0;
	  if (counter == DRAW_TIME_SAMPLING_POINT){
		// save draw time (MODE5/6: JPEG, MODE7: YUV->RGB and RGB)
		drawTime = (currentTime.toMSecsSinceEpoch() - startDrawTime.toMSecsSinceEpoch())*1000;
		counter = 0;
		//cout << "[" << name << "] drawTime : " << drawTime << " (us)" << endl;
	  }
	  else {
		counter++;
	  }
	}

#if 0 // for TEST
	cout << "[" << name << "] drawTime : " << drawTime << endl;
	cout << "[" << name << "] pastTime : " << pastTime << endl;
	cout << "[" << name << "] interval : " << interval << endl;
#endif // 1 // for TEST
	if (pastTime < interval){
	  unsigned long sleepTime = interval - pastTime;
	  //	  cout << "[" << name << "] sleepTime: " <<  sleepTime << endl << flush;
	  usleep(sleepTime);
	}
	else {
	  // No wait
	  //	  cout << "[" << name << "] sleepTime: 0" << endl << flush;
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

  // average draw frame time
  averageDrawFrameTime = 0;

  // draw time
  drawTime = 0;

  // reset previous frame time to Null
  previousGetFrameRateTime = QDateTime();

  NetThread::connectedToServer();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void GraphicsThread::shutdownConnection()
{
  // reset previous frame time to Null
  previousGetFrameRateTime = QDateTime();

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
#if QTB_PUBLIC_MODE7_SUPPORT
// setup for yuv420, rgb24
inline bool GraphicsThread::setup()
{
  int yuvBufferSize = width * height + width * height / 2;
  // allocate yuv420/rgb24 buffer
  if (yuv1 != 0){
	delete [] yuv1;
  }
  yuv1 = new uchar[yuvBufferSize];
  memset(yuv1, 0, yuvBufferSize);
  if (yuv2 != 0){
	delete [] yuv2;
  }
  yuv2 = new uchar[yuvBufferSize];
  memset(yuv2, 0xFF, yuvBufferSize);
#if USE_PPM_LOADER_FOR_VP8
  if (ppm != 0){
	delete [] ppm;
  }
  ppm = new uchar[width * height * 3 + PPM_HEADER_SIZE_MAX];
  // make PPM header
  int length = snprintf((char*)ppm, PPM_HEADER_SIZE_MAX, PPM_HEADER_FORMAT, width, height);
  // set rgb24
  rgb24 = ppm + length;
#else // USE_PPM_LOADER_FOR_VP8
  if (rgb24 != 0){
	delete [] rgb24;
  }
  rgb24 = new uchar[width * height * IMAGE_FORMAT_SIZE];
#endif // USE_PPM_LOADER_FOR_VP8

  // calc parameters
  hwidth = width / 2;
  size = width * height;
  y1topOrg = (uchar*)yuv1;
  u1topOrg = y1topOrg + size;
  v1topOrg = u1topOrg + size / 4;
  y2topOrg = (uchar*)yuv2;
  u2topOrg = y2topOrg + size;
  v2topOrg = u2topOrg + size / 4;
  yuv420 = yuv1;
  uvNext = width / 2;
  rgb24Next = - width * IMAGE_FORMAT_SIZE * 2;
#if QTB_MULTI_THREAD_CONVERTER
  // set for qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
  qtbrynhildr::width = width;
  qtbrynhildr::uvNext = uvNext;
  qtbrynhildr::rgb24Next = rgb24Next;
  qtbrynhildr::yuv420 = yuv420;
  qtbrynhildr::yuv1 = yuv1;
  qtbrynhildr::y1topOrg = y1topOrg;
  qtbrynhildr::u1topOrg = u1topOrg;
  qtbrynhildr::v1topOrg = v1topOrg;
  qtbrynhildr::y2topOrg = y2topOrg;
  qtbrynhildr::u2topOrg = u2topOrg;
  qtbrynhildr::v2topOrg = v2topOrg;
#endif // QTB_MULTI_THREAD_CONVERTER

  return true;
}

// make YUV420 image
inline bool GraphicsThread::makeYUV420Image()
{
  // get 1 frame image (YUV420)
  vpx_codec_iter_t iter = 0;
  vpx_image_t *img = vpx_codec_get_frame(&c_codec, &iter);
  if (img == 0){
	return false;
  }

  // set size
  if (width  != (int)img->d_w ||
	  height != (int)img->d_h){
	// set new width/height
	width  = img->d_w;
	height = img->d_h;

	// setup for yuv420, rgb24
	if (!setup()){
	  return false;
	}
  }

  // switch yuv420 buffer
  if (yuv420 == yuv1){
	yuv420 = yuv2;
  }
  else {
	yuv420 = yuv1;
  }
  qtbrynhildr::yuv420 = yuv420;

  // create yuv420
  uchar *top = yuv420;

  // Y
  uchar *buf = img->planes[0];
  int stride = img->stride[0];
  for(int yPos = 0; yPos < height; yPos++){
	memcpy(top, buf, width);
	top += width;
	buf += stride;
  }
  // U
  buf = img->planes[1];
  stride = img->stride[1];
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, hwidth);
	top += hwidth;
	buf += stride;
  }
  // V
  buf = img->planes[2];
  stride = img->stride[2];
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, hwidth);
	top += hwidth;
	buf += stride;
  }

  return true;
}

// make RGB24 image
inline int GraphicsThread::makeRGB24Image()
{
  // make yuv420 image
  if (!makeYUV420Image()){
	return 0;
  }

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread 1 or 2 or 4
  int numOfThread = settings->getConvertThreadCount();
  uchar *rgb24top = rgb24 + width * (height - 1) * IMAGE_FORMAT_SIZE;
  uchar *ytop;
  uchar *utop;
  uchar *vtop;
  if (yuv420 == yuv1){
	ytop = y1topOrg;
	utop = u1topOrg;
	vtop = v1topOrg;
  }
  else {
	ytop = y2topOrg;
	utop = u2topOrg;
	vtop = v2topOrg;
  }
  // 1 thread version
  if (numOfThread <= 1 || height % 2 != 0){
	// convert YUV420 to RGB24
	convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
	return size * IMAGE_FORMAT_SIZE;
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<void> f1, f2, f3;
	int linesOfThread = height / numOfThread;

	// start 1st thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);

	if (numOfThread > 2){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 2nd thread
	  f2 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for 3rd thread
	if (numOfThread > 3){
	  // for next thread
	  rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start 3rd thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for next thread
	rgb24top -= (width * linesOfThread) * IMAGE_FORMAT_SIZE;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// for last thread (GraphicsThread::convertYUV420toRGB24())
	convertYUV420toRGB24(ytop, utop, vtop, rgb24top, linesOfThread);

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();

	return size * IMAGE_FORMAT_SIZE;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV420 to RGB24
  convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
  return size * IMAGE_FORMAT_SIZE;
#endif // QTB_MULTI_THREAD_CONVERTER
}

#endif // QTB_PUBLIC_MODE7_SUPPORT

} // end of namespace qtbrynhildr
