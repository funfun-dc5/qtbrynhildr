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

// qtbrynhhildr::clip() (NOT GraphicsThread::clip())
// clip
inline int clip(int val)
{
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);
#endif // QTB_MULTI_THREAD_CONVERTER

// yuv420toRGB24 (select one)
// =========================================================
// YUV420TORGB24_VERSION 1 : float                   84 fps
// YUV420TORGB24_VERSION 2 : integer and shift      129 fps
// YUV420TORGB24_VERSION 3 : V2 + small improvement 131 fps
// YUV420TORGB24_VERSION 4 : V3 + small improvement 132 fps
// YUV420TORGB24_VERSION 5 : V4 + loop unrolling    136 fps
// YUV420TORGB24_VERSION 6 : integer (SSE)          158 fps
// YUV420TORGB24_VERSION 7 : float   (AVX)          xxx fps (SLOW)
// =========================================================
#if QTB_USE_SIMD
#define YUV420TORGB24_VERSION 6
#else // QTB_USE_SIMD
#define YUV420TORGB24_VERSION 4
#endif // QTB_USE_SIMD

#if QTB_MULTI_THREAD_CONVERTER
// 1 (2 threads)     - 175 fps
// 5 (2 threads)     - 192 fps
// 6 (2 threads:SSE) - xxx fps
#if QTB_USE_SIMD
#define YUV420TORGB24_MT_VERSION 6
#else // QTB_USE_SIMD
#define YUV420TORGB24_MT_VERSION 1
#endif // QTB_USE_SIMD
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
  yuv420prev(0),
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
#if 0 // for TEST
	else {
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 pastTime = (currentTime.toMSecsSinceEpoch() - startDrawFrameTime.toMSecsSinceEpoch())*1000;
	  cout << "[" << name << "] NETWORK : " << pastTime << " (us)" << endl;
	}
#endif
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

#if 0 // for TEST
  // frame skip check
  if (settings->getOnGraphics()){
	// frame rate control
	if (QTB_DESKTOP_FRAMERATE_CONTROL && drawTime != 0){
	  QDateTime currentTime = QDateTime::currentDateTime();
	  qint64 pastTime = (QTB_THREAD_SLEEP_TIME +
						 currentTime.toMSecsSinceEpoch() - startDrawFrameTime.toMSecsSinceEpoch())*1000;
	  qint64 threshold = averageDrawFrameTime; // settings->getFrameInterval();

	  //cout << "[" << name << "] pastTime  : " << pastTime << " (us)" << endl << flush;
	  //cout << "[" << name << "] drawTime  : " << drawTime << " (us)" << endl;
	  //cout << "[" << name << "] threshold : " << threshold << " (us)" << endl;
	  if (pastTime + drawTime > threshold){
		// drop this frame
		cout << "pastTime + drawTime > threshold" << endl;
		cout << "[" << name << "] pastTime  : " << pastTime << " (us)" << endl;
		cout << "[" << name << "] drawTime  : " << drawTime << " (us)" << endl;
		cout << "[" << name << "] threshold : " << threshold << " (us)" << endl << flush;
	  }
	}
  }
#endif

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

	//cout << "[" << name << "] drawTime : " << drawTime << " (us)" << endl;
	//cout << "[" << name << "] pastTime  : " << pastTime << " (us)" << endl << flush;
	//cout << "[" << name << "] interval : " << interval << " (us)" << endl << flush;
	if (pastTime < interval){
	  unsigned long sleepTime = interval - pastTime;
	  //cout << "[" << name << "] sleepTime : " <<  sleepTime << " (us)" << endl << flush;
	  usleep(sleepTime);
	}
	else {
	  // No wait
	  //cout << "[" << name << "] No Sleep" << endl << flush;
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
  // allocate yuv420/rgb24 buffer
  if (yuv1 != 0){
	delete [] yuv1;
  }
  yuv1 = new uchar[width * height + width * height / 2];
  if (yuv2 != 0){
	delete [] yuv2;
  }
  yuv2 = new uchar[width * height + width * height / 2];
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
  yuv420prev = yuv2;
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

  // create yuv420
  if (yuv420 == yuv1){
	yuv420 = yuv2;
	yuv420prev = yuv1;
  }
  else {
	yuv420 = yuv1;
	yuv420prev = yuv2;
  }
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
	return convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
  }
  else { // numOfThread >= 2
	// 2 thread or 4 thread version
	QFuture<int> f1, f2, f3;
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
  return convertYUV420toRGB24(ytop, utop, vtop, rgb24top, height);
#endif // QTB_MULTI_THREAD_CONVERTER
}

#if QTB_MULTI_THREAD_CONVERTER

// 1 (2 threads)     - 175 fps
// 5 (2 threads)     - 192 fps
// 6 (2 threads:SSE) - xxx fps

#if YUV420TORGB24_MT_VERSION == 1

// YUV420 convert to RGB macro
#define GET_R_MT(Y, V)		((256 * Y           + 358 * V) >> 8)
#define GET_G_MT(Y, U, V)	((256 * Y -  88 * U - 182 * V) >> 8)
#define GET_B_MT(Y, U)		((256 * Y + 453 * U          ) >> 8)

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (qtbrynhildr::yuv420 == qtbrynhildr::yuv1){
	yptop = qtbrynhildr::y2topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u2topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v2topOrg + (vtop - qtbrynhildr::yuv420);
  }
  else {
	yptop = qtbrynhildr::y1topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u1topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v1topOrg + (vtop - qtbrynhildr::yuv420);
  }

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < qtbrynhildr::width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);

	  // == xPos ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = qtbrynhildr::clip(GET_R_MT(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = qtbrynhildr::clip(GET_G_MT(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = qtbrynhildr::clip(GET_B_MT(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = qtbrynhildr::clip(GET_R_MT(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = qtbrynhildr::clip(GET_G_MT(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = qtbrynhildr::clip(GET_B_MT(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += qtbrynhildr::rgb24Next;
	if (yPos & 0x1){
	  utop += qtbrynhildr::uvNext;
	  vtop += qtbrynhildr::uvNext;
	  uptop += qtbrynhildr::uvNext;
	  vptop += qtbrynhildr::uvNext;
	}
  }
  return rgb24size;
}
#endif // YUV420TORGB24_MT_VERSION == 1

#if YUV420TORGB24_MT_VERSION == 5

// YUV420 convert to RGB macro
#define GET_VALUE(Y, X)	((Y + X) >> 8)

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < qtbrynhildr::width; xPos += 4, uvOffset += 2){
	  uchar *utop1 = utop + uvOffset;
	  uchar *vtop1 = vtop + uvOffset;
	  int r, g, b;
	  int y, u, v;
	  int r1,g1,b1;

	  // set u/v
	  u = *utop1++ - 128;
	  v = *vtop1++ - 128;

	  // calc temporary value
	  r1 = 358 * v;
	  g1 = -88 * u - 182 * v;
	  b1 = 453 * u;

	  // == xPos ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = qtbrynhildr::clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = qtbrynhildr::clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888


	  // check width
	  if (xPos + 2 >= width)
		break;

	  // set u/v
	  u = *utop1 - 128;
	  v = *vtop1 - 128;

	  // calc temporary value
	  r1 = 358 * v;
	  g1 = -88 * u - 182 * v;
	  b1 = 453 * u;

	  // == xPos ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = qtbrynhildr::clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = qtbrynhildr::clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  rgb24size += IMAGE_FORMAT_SIZE * 4;
	}
	rgb24top += qtbrynhildr::rgb24Next;
	if (yPos & 0x1){
	  utop += qtbrynhildr::uvNext;
	  vtop += qtbrynhildr::uvNext;
	}
  }
  return rgb24size;
}

#endif // YUV420TORGB24_MT_VERSION == 5

#if YUV420TORGB24_MT_VERSION == 6

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#else // defined(__ARM_NEON__)
#if defined(_MSC_VER)
#include <intrin.h>
#else // defined(_MSC_VER)
#include <x86intrin.h>
#endif // defined(_MSC_VER)
#endif // defined(__ARM_NEON__)

// for align
#if defined(_MSC_VER)
#define Aligned(n)  __declspec(align(n))
#else // defined(_MSC_VER)
#define Aligned(n)  __attribute__((aligned(n)))
#endif // defined(_MSC_VER)

// SSE

// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
#if defined(__ARM_NEON__)

int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  int result[4] Aligned(16);

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (qtbrynhildr::yuv420 == qtbrynhildr::yuv1){
	yptop = qtbrynhildr::y2topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u2topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v2topOrg + (vtop - qtbrynhildr::yuv420);
  }
  else {
	yptop = qtbrynhildr::y1topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u1topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v1topOrg + (vtop - qtbrynhildr::yuv420);
  }

  const int yca[4] Aligned(16) = {256,  256, 256, 0};
  const int uca[4] Aligned(16) = {0,    -88, 453, 0};
  const int vca[4] Aligned(16) = {358, -182,   0, 0};

  int32x4_t yc = vld1q_s32(yca);
  int32x4_t uc = vld1q_s32(uca);
  int32x4_t vc = vld1q_s32(vca);

  int32x4_t constMaxV = vdupq_n_s32(255);
  int32x4_t constMinV = vdupq_n_s32(0);

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int32x4_t yv, uv, vv;
	  int32x4_t uv0, vv0;

	  // set u/v
	  u = (int)(*(utop + uvOffset) - 128);
	  v = (int)(*(vtop + uvOffset) - 128);
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);

	  // load U, V
	  uv0 = vdupq_n_s32(u);
	  vv0 = vdupq_n_s32(v);

	  // xPos

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // xPos+1

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	}
  }
  return rgb24size;
}

#else // defined(__ARM_NEON__)

int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  int result[4] Aligned(16);

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (qtbrynhildr::yuv420 == qtbrynhildr::yuv1){
	yptop = qtbrynhildr::y2topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u2topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v2topOrg + (vtop - qtbrynhildr::yuv420);
  }
  else {
	yptop = qtbrynhildr::y1topOrg + (ytop - qtbrynhildr::yuv420);
	uptop = qtbrynhildr::u1topOrg + (utop - qtbrynhildr::yuv420);
	vptop = qtbrynhildr::v1topOrg + (vtop - qtbrynhildr::yuv420);
  }

#if 1 // for TEST

  __m128i yc = _mm_setr_epi32(256, 256, 256, 0);
  __m128i uc = _mm_setr_epi32(0,   -88, 453, 0);
  __m128i vc = _mm_setr_epi32(358,-182,   0, 0);

  __m128i constMaxV = _mm_setr_epi32(255, 255, 255, 255);
  //  __m128i constMinV = _mm_setr_epi32(0, 0, 0, 0);
  __m128i constMinV = _mm_setzero_si128();

#else // for TEST

  const int constYc[4] Aligned(16) = {256, 256, 256, 0};
  const int constUc[4] Aligned(16) = {0,   -88, 453, 0};
  const int constVc[4] Aligned(16) = {358,-182,   0, 0};

  // 2) load Yc, Uc, Vc
  __m128i yc = _mm_load_si128((const __m128i*)constYc);
  __m128i uc = _mm_load_si128((const __m128i*)constUc);
  __m128i vc = _mm_load_si128((const __m128i*)constVc);

  const int constMax[4] Aligned(16) = {255, 255, 255, 255};
  const int constMin[4] Aligned(16) = {  0,   0,   0,   0};

  __m128i constMaxV = _mm_load_si128((const __m128i*)constMax);
  __m128i constMinV = _mm_load_si128((const __m128i*)constMin);

#endif // for TEST

  int ya[4] Aligned(16) = {  0,   0,   0,   0};
  int ua[4] Aligned(16) = {  0,   0,   0,   0};
  int va[4] Aligned(16) = {  0,   0,   0,   0};

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < qtbrynhildr::width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  __m128i yv, uv, vv;
	  __m128i uv0, vv0;

	  // set u/v
	  u = (int)(*(utop + uvOffset) - 128);
	  v = (int)(*(vtop + uvOffset) - 128);
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);
	  ua[0] = u;
	  ua[1] = u;
	  ua[2] = u;
	  va[0] = v;
	  va[1] = v;
	  va[2] = v;

	  // load U, V
	  uv0 = _mm_load_si128((const __m128i*)ua);
	  vv0 = _mm_load_si128((const __m128i*)va);

	  // xPos

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // xPos+1

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += qtbrynhildr::rgb24Next;
	if (yPos & 0x1){
	  utop += qtbrynhildr::uvNext;
	  vtop += qtbrynhildr::uvNext;
	  uptop += qtbrynhildr::uvNext;
	  vptop += qtbrynhildr::uvNext;
	}
  }
  return rgb24size;
}

#endif // defined(__ARM_NEON__)

#endif // YUV420TORGB24_MT_VERSION == 6

#endif  // QTB_MULTI_THREAD_CONVERTER

// =========================================================
// YUV420TORGB24_VERSION 1 : float                   84 fps
// YUV420TORGB24_VERSION 2 : integer and shift      129 fps
// YUV420TORGB24_VERSION 3 : V2 + small improvement 131 fps
// YUV420TORGB24_VERSION 4 : V3 + small improvement 132 fps
// YUV420TORGB24_VERSION 5 : V4 + loop unrolling    136 fps
// YUV420TORGB24_VERSION 6 : integer (SSE)          158 fps
// YUV420TORGB24_VERSION 7 : float   (AVX)          xxx fps
// =========================================================

#if YUV420TORGB24_VERSION == 1 || YUV420TORGB24_VERSION == 2
// YUV420 convert to RGB macro
#if YUV420TORGB24_VERSION == 1 // float (original)
#define GET_R(Y, V)		(Y             + 1.402 * V)
#define GET_G(Y, U, V)	(Y - 0.344 * U - 0.714 * V)
#define GET_B(Y, U)		(Y + 1.772 * U            )
#endif // YUV420TORGB24_VERSION == 1
#if YUV420TORGB24_VERSION == 2 // integer and shift
#define GET_R(Y, V)		((256 * Y           + 358 * V) >> 8)
#define GET_G(Y, U, V)	((256 * Y -  88 * U - 182 * V) >> 8)
#define GET_B(Y, U)		((256 * Y + 453 * U          ) >> 8)
#endif // YUV420TORGB24_VERSION == 2

// convert YUV420 to RGB24
int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv420 == yuv1){
	yptop = y2topOrg + (ytop - yuv420);
	uptop = u2topOrg + (utop - yuv420);
	vptop = v2topOrg + (vtop - yuv420);
  }
  else {
	yptop = y1topOrg + (ytop - yuv420);
	uptop = u1topOrg + (utop - yuv420);
	vptop = v1topOrg + (vtop - yuv420);
  }

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);

	  // == xPos ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
  return rgb24size;
}
#endif // YUV420TORGB24_VERSION == 1 || YUV420TORGB24_VERSION == 2


#if YUV420TORGB24_VERSION == 3
// YUV420 convert to RGB macro
#define GET_R(Y, V)		((Y           + 358 * V) >> 8)
#define GET_G(Y, U, V)	((Y -  88 * U - 182 * V) >> 8)
#define GET_B(Y, U)		((Y + 453 * U          ) >> 8)

// convert YUV420 to RGB24
int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv420 == yuv1){
	yptop = y2topOrg + (ytop - yuv420);
	uptop = u2topOrg + (utop - yuv420);
	vptop = v2topOrg + (vtop - yuv420);
  }
  else {
	yptop = y1topOrg + (ytop - yuv420);
	uptop = u1topOrg + (utop - yuv420);
	vptop = v1topOrg + (vtop - yuv420);
  }

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);

	  // == xPos ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
  return rgb24size;
}
#endif // YUV420TORGB24_VERSION == 3

#if YUV420TORGB24_VERSION == 4 || YUV420TORGB24_VERSION == 5
// YUV420 convert to RGB macro
#define GET_VALUE(Y, X)	((Y + X) >> 8)

#if YUV420TORGB24_VERSION == 4
// convert YUV420 to RGB24
int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  for (int yPos = 0; yPos < height; yPos++){
	//	cout << "yPos = " << yPos << endl << flush;
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int r1,g1,b1;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;

	  // calc temporary value
	  r1 = 358 * v;
	  g1 = -88 * u - 182 * v;
	  b1 = 453 * u;

	  // == xPos ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	}
  }
  return rgb24size;
}
#endif // YUV420TORGB24_VERSION == 4

#if YUV420TORGB24_VERSION == 5
// convert YUV420 to RGB24
int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 4, uvOffset += 2){
	  uchar *utop1 = utop + uvOffset;
	  uchar *vtop1 = vtop + uvOffset;
	  int r, g, b;
	  int y, u, v;
	  int r1,g1,b1;

	  // set u/v
	  u = *utop1++ - 128;
	  v = *vtop1++ - 128;

	  // calc temporary value
	  r1 = 358 * v;
	  g1 = -88 * u - 182 * v;
	  b1 = 453 * u;

	  // == xPos ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // check width
	  if (xPos + 2 >= width)
		break;

	  // set u/v
	  u = *utop1 - 128;
	  v = *vtop1 - 128;

	  // calc temporary value
	  r1 = 358 * v;
	  g1 = -88 * u - 182 * v;
	  b1 = 453 * u;

	  // == xPos ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // == xPos+1 ==
	  y = (int)*ytop++;
	  y <<= 8; // y * 256

	  // R
	  r = clip(GET_VALUE(y, r1));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_VALUE(y, g1));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_VALUE(y, b1));
	  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  rgb24size += IMAGE_FORMAT_SIZE * 4;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	}
  }
  return rgb24size;
}
#endif // YUV420TORGB24_VERSION == 5
#endif // YUV420TORGB24_VERSION == 4 || YUV420TORGB24_VERSION == 5

#if YUV420TORGB24_VERSION == 6

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#else // defined(__ARM_NEON__)
#if defined(_MSC_VER)
#include <intrin.h>
#else // defined(_MSC_VER)
#include <x86intrin.h>
#endif // defined(_MSC_VER)
#endif // defined(__ARM_NEON__)

// for align
#if defined(_MSC_VER)
#define Aligned(n)  __declspec(align(n))
#else // defined(_MSC_VER)
#define Aligned(n)  __attribute__((aligned(n)))
#endif // defined(_MSC_VER)

// SSE

// convert YUV420 to RGB24
#if defined(__ARM_NEON__)

int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  int result[4] Aligned(16);

  const int yca[4] Aligned(16) = {256,  256, 256, 0};
  const int uca[4] Aligned(16) = {0,    -88, 453, 0};
  const int vca[4] Aligned(16) = {358, -182,   0, 0};

  int32x4_t yc = vld1q_s32(yca);
  int32x4_t uc = vld1q_s32(uca);
  int32x4_t vc = vld1q_s32(vca);

  int32x4_t constMaxV = vdupq_n_s32(255);
  int32x4_t constMinV = vdupq_n_s32(0);

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv420 == yuv1){
	yptop = y2topOrg + (ytop - yuv420);
	uptop = u2topOrg + (utop - yuv420);
	vptop = v2topOrg + (vtop - yuv420);
  }
  else {
	yptop = y1topOrg + (ytop - yuv420);
	uptop = u1topOrg + (utop - yuv420);
	vptop = v1topOrg + (vtop - yuv420);
  }

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  int32x4_t yv, uv, vv;
	  int32x4_t uv0, vv0;

	  // set u/v
	  u = (int)(*(utop + uvOffset) - 128);
	  v = (int)(*(vtop + uvOffset) - 128);
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);
	  // load U, V
	  uv0 = vdupq_n_s32(u);
	  vv0 = vdupq_n_s32(v);

	  // xPos

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // xPos+1

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
  return rgb24size;
}

#else // defined(__ARM_NEON__)

int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  int result[4] Aligned(16);

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv420 == yuv1){
	yptop = y2topOrg + (ytop - yuv420);
	uptop = u2topOrg + (utop - yuv420);
	vptop = v2topOrg + (vtop - yuv420);
  }
  else {
	yptop = y1topOrg + (ytop - yuv420);
	uptop = u1topOrg + (utop - yuv420);
	vptop = v1topOrg + (vtop - yuv420);
  }

#if 1 // for TEST

  __m128i yc = _mm_setr_epi32(256, 256, 256, 0);
  __m128i uc = _mm_setr_epi32(0,   -88, 453, 0);
  __m128i vc = _mm_setr_epi32(358,-182,   0, 0);

  __m128i constMaxV = _mm_setr_epi32(255, 255, 255, 255);
  //  __m128i constMinV = _mm_setr_epi32(0, 0, 0, 0);
  __m128i constMinV = _mm_setzero_si128();

#else // for TEST

  const int constYc[4] Aligned(16) = {256, 256, 256, 0};
  const int constUc[4] Aligned(16) = {0,   -88, 453, 0};
  const int constVc[4] Aligned(16) = {358,-182,   0, 0};

  // 2) load Yc, Uc, Vc
  __m128i yc = _mm_load_si128((const __m128i*)constYc);
  __m128i uc = _mm_load_si128((const __m128i*)constUc);
  __m128i vc = _mm_load_si128((const __m128i*)constVc);

  const int constMax[4] Aligned(16) = {255, 255, 255, 255};
  const int constMin[4] Aligned(16) = {  0,   0,   0,   0};

  __m128i constMaxV = _mm_load_si128((const __m128i*)constMax);
  __m128i constMinV = _mm_load_si128((const __m128i*)constMin);

#endif // for TEST

  int ya[4] Aligned(16) = {  0,   0,   0,   0};
  int ua[4] Aligned(16) = {  0,   0,   0,   0};
  int va[4] Aligned(16) = {  0,   0,   0,   0};

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  __m128i yv, uv, vv;
	  __m128i uv0, vv0;

	  // set u/v
	  u = (int)(*(utop + uvOffset) - 128);
	  v = (int)(*(vtop + uvOffset) - 128);
	  up = (int)(*(uptop + uvOffset) - 128);
	  vp = (int)(*(vptop + uvOffset) - 128);
	  ua[0] = u;
	  ua[1] = u;
	  ua[2] = u;
	  va[0] = v;
	  va[1] = v;
	  va[2] = v;

	  // load U, V
	  uv0 = _mm_load_si128((const __m128i*)ua);
	  vv0 = _mm_load_si128((const __m128i*)va);

	  // xPos

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // xPos+1

	  // set y
	  y = (int)(*ytop++);
	  yp = (int)(*yptop++);
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
  return rgb24size;
}

#endif // defined(__ARM_NEON__)

#endif // YUV420TORGB24_VERSION == 6

#if YUV420TORGB24_VERSION == 7

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#else // defined(__ARM_NEON__)
#if defined(_MSC_VER)
#include <intrin.h>
#else // defined(_MSC_VER)
#include <x86intrin.h>
#endif // defined(_MSC_VER)
#endif // defined(__ARM_NEON__)

// for align
#if defined(_MSC_VER)
#define Aligned(n)  __declspec(align(n))
#else // defined(_MSC_VER)
#define Aligned(n)  __attribute__((aligned(n)))
#endif // defined(_MSC_VER)

// AVX

// convert YUV420 to RGB24
int GraphicsThread::convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

#if defined(__ARM_NEON__)

  // Yet
#error "NOT support AVX for ARM NEON"

#else // defined(__ARM_NEON__)

  int result[8] Aligned(32);

  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv420 == yuv1){
	yptop = y2topOrg + (ytop - yuv420);
	uptop = u2topOrg + (utop - yuv420);
	vptop = v2topOrg + (vtop - yuv420);
  }
  else {
	yptop = y1topOrg + (ytop - yuv420);
	uptop = u1topOrg + (utop - yuv420);
	vptop = v1topOrg + (vtop - yuv420);
  }

#if 1 // for TEST

  // 4) load Uk
  __m256 uk = _mm256_setr_ps(0, -0.34, 1.72, 0, 0, -0.34, 1.72, 0);
  // 5) load Vk
  __m256 vk = _mm256_setr_ps(1.402, -0.714, 0, 0, 1.402, -0.714, 0, 0);

  __m256 constMaxV = _mm256_setr_ps(255,255,255,255,255,255,255,255);
  __m256 constMinV = _mm256_setzero_ps();

#else // for TEST

  const float constUc[8] Aligned(32) = {0, -0.34, 1.72, 0, 0, -0.34, 1.72, 0};
  const float constVc[8] Aligned(32) = {1.402, -0.714, 0, 0, 1.402, -0.714, 0, 0};

  const float constMax = 255.0;
  const float constMin = 0.0;

  // 4) load Uc
  __m256 uc = _mm256_load_ps(constUc);
  // 5) load Vc
  __m256 vc = _mm256_load_ps(constVc);

  __m256 constMaxV = _mm256_broadcast_ss(&constMax);
  __m256 constMinV = _mm256_broadcast_ss(&constMin);

#endif // 1 // for TEST

  // set RC flag
  unsigned long mxcsr = _mm_getcsr();
  mxcsr &= 0xFFFF9FFF;
  //  mxcsr |= 0x00000000;
  //  mxcsr |= 0x00002000;
  //  mxcsr |= 0x00004000;
  mxcsr |= 0x00006000;
  _mm_setcsr(mxcsr);

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  float y[8] Aligned(32);
	  float y1, y2, u, v;
	  __m256 yv, uv, vv;
	  __m256i yvi;

	  // set y[8]
	  y1 = (float)(*ytop++);
	  y2 = (float)(*ytop++);
	  y[0] = y1;
	  y[1] = y1;
	  y[2] = y1;
	  y[3] = 0.0;
	  y[4] = y2;
	  y[5] = y2;
	  y[6] = y2;
	  y[7] = 0.0;

	  // set u/v
	  u = (float)(*(utop + uvOffset) - 128);
	  v = (float)(*(vtop + uvOffset) - 128);

	  // ============================================================
	  // 1) load Y vector
	  yv = _mm256_load_ps(y);
	  // 2) load U vector
	  uv = _mm256_broadcast_ss(&u);
	  // 3) load V vector
	  vv = _mm256_broadcast_ss(&v);

	  // 4) U * Uc -> U
	  uv = _mm256_mul_ps(uv, uc);
	  // 5) V * Vc -> V
	  vv = _mm256_mul_ps(vv, vc);

	  // 6) Y + U + V -> Y
	  yv = _mm256_add_ps(yv, uv); // Y + U -> Y
	  yv = _mm256_add_ps(yv, vv); // Y + V -> Y

	  // 7) Y > 255 ? 255 : Y
	  yv = _mm256_min_ps(yv, constMaxV);
	  // 8) Y < 0 ? 0 : Y
	  yv = _mm256_max_ps(yv, constMinV);

	  // 9) convert float to integer
	  yvi = _mm256_cvtps_epi32(yv);

	  // 10) store to result
	  _mm256_store_si256((__m256i*)result, yvi);
	  // ============================================================

	  // set rgba32 * 2 from result int * 8

	  // xPos
	  // R
	  *rgb24top++ = (uchar)result[0];

	  // G
	  *rgb24top++ = (uchar)result[1];

	  // B
	  *rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  // xPos+1
	  // R
	  *rgb24top++ = (uchar)result[4];

	  // G
	  *rgb24top++ = (uchar)result[5];

	  // B
	  *rgb24top++ = (uchar)result[6];

#if FORMAT_RGBA8888
	  // A
	  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

	  rgb24size += IMAGE_FORMAT_SIZE * 2;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
#endif // defined(__ARM_NEON__)
  return rgb24size;
}

#endif // YUV420TORGB24_VERSION == 7

#endif // QTB_PUBLIC_MODE7_SUPPORT

} // end of namespace qtbrynhildr
