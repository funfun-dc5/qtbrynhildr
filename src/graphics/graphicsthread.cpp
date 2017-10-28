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
int width;
int uvNext;
int rgb24Next;

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

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
GraphicsThread::GraphicsThread(Settings *settings, DesktopWindow *desktopWindow)
  :
  NetThread("GraphicsThread", settings, desktopWindow),
  image(0),
  frameCounter(0),
  averageDrawFrameTime(0),
  totalFrameCounter(0),
  drawTime(0),
  onClearDesktop(false),
#if QTB_PUBLIC_MODE7_SUPPORT
  width(0),
  height(0),
  yuv420(0),
#if USE_PPM_LOADER_FOR_VP8
  ppm(0),
#endif // USE_PPM_LOADER_FOR_VP8
  rgb24(0),
  doneVpxInit(false),
#endif // QTB_PUBLIC_MODE7_SUPPORT
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

#if QTB_PUBLIC_MODE7_SUPPORT
  // buffer for yuv420/rgb24
  if (yuv420 != 0){
	delete [] yuv420;
	yuv420 = 0;
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
  if (!settings->getOnGraphics()) return 0.0;

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
	if (com_data->video_mode == VIDEO_MODE_MJPEG){
	  // load a JPEG data to desktop
	  desktopLoadResult = image->loadFromData((const uchar *)buffer,
											  (uint)receivedDataSize,
											  "JPEG");
	}
#if QTB_PUBLIC_MODE7_SUPPORT
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
		  image = new QImage(width, height, QImage::Format_RGB888);
		  image->fill(Qt::gray);
		}
	  }
#else // USE_PPM_LOADER_FOR_VP8
	  int rgb24size = makeRGB24Image();
	  if (rgb24size != 0){
		// create QImage from RGB24
		delete image;
		image = new QImage(rgb24, width, height, QImage::Format_RGB888);
	  }
	  else {
		if (image->isNull()){
		  delete image;
		  image = new QImage(width, height, QImage::Format_RGB888);
		  image->fill(Qt::gray);
		}
	  }

	  desktopLoadResult = true;
#endif // USE_PPM_LOADER_FOR_VP8
	}
	else {
	  // illegal VIDEO_MODE
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
		// save draw time (MODE5/6: JPEG, MODE7: YUV->RGB and RGB888)
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
// make RGB24 image
int GraphicsThread::makeRGB24Image()
{
  // get 1 frame image (YUV420)
  vpx_codec_iter_t iter = 0;
  vpx_image_t *img = vpx_codec_get_frame(&c_codec, &iter);
  if (img == 0) {
	return 0;
  }

  // set size
  if (width  != (int)img->d_w ||
	  height != (int)img->d_h){
	// set new width/height
	width  = img->d_w;
	height = img->d_h;
	//  cout << "width = " << width << endl << "height = " << height << endl << flush;

	// allocate yuv420/rgb24 buffer
	if (yuv420 != 0){
	  delete [] yuv420;
	}
	yuv420 = new uchar[width * height + width * height / 2];
#if USE_PPM_LOADER_FOR_VP8
	if (ppm != 0){
	  delete [] ppm;
	}
	ppm = new uchar[width * height * 3 + PPM_HEADER_SIZE_MAX];
	// make PPM header
	snprintf((char*)ppm, PPM_HEADER_SIZE_MAX, PPM_HEADER_FORMAT, width, height);
	// set rgb24 top
	rgb24 = ppm + strlen((char*)ppm);
#else // USE_PPM_LOADER_FOR_VP8
	if (rgb24 != 0){
	  delete [] rgb24;
	}
	rgb24 = new uchar[width * height * 3];
#endif // USE_PPM_LOADER_FOR_VP8

	// calc parameters
	hwidth = width / 2;
	size = width * height;
	ytopOrg = (uchar*)yuv420;
	utopOrg = ytopOrg + size;
	vtopOrg = utopOrg + size / 4;
	uvNext = width / 2;
	rgb24Next = - width * 3 * 2;
#if QTB_MULTI_THREAD_CONVERTER
	// set for qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
	qtbrynhildr::width = width;
	qtbrynhildr::uvNext = uvNext;
	qtbrynhildr::rgb24Next = rgb24Next;
#endif // QTB_MULTI_THREAD_CONVERTER
  }

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

#if QTB_MULTI_THREAD_CONVERTER
  // number of thread
  int numOfThread = settings->getConvertThreadCount();
  //  int numOfThread = 0; // 0 or 1 or 2 or 4
  if (numOfThread <= 1 || height % 2 != 0){
	// convert YUV420 to RGB24
	return convertYUV420toRGB24();
  }
  else { // numOfThread >= 2
	// 1 thread or 2 thread or 4 thread version
	QFuture<int> f1, f2, f3, f4;
	int linesOfThread = height / numOfThread;

	// for 1st thread
	uchar *rgb24top = rgb24 + width * (height - 1) * 3;
	uchar *ytop = ytopOrg;
	uchar *utop = utopOrg;
	uchar *vtop = vtopOrg;

	// start thread
	f1 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);

	// for 2nd thread
	rgb24top -= (width * linesOfThread) * 3;
	ytop += width * linesOfThread;
	utop += uvNext * linesOfThread/2;
	vtop += uvNext * linesOfThread/2;

	// start thread
	f2 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);

	// for 3rd thread
	if (numOfThread >= 4){
	  rgb24top -= (width * linesOfThread) * 3;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start thread
	  f3 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// for 4th thread
	if (numOfThread >= 4){
	  rgb24top -= (width * linesOfThread) * 3;
	  ytop += width * linesOfThread;
	  utop += uvNext * linesOfThread/2;
	  vtop += uvNext * linesOfThread/2;

	  // start thread
	  f4 = QtConcurrent::run(qtbrynhildr::convertYUV420toRGB24, ytop, utop, vtop, rgb24top, linesOfThread);
	}

	// wait for all threads finished
	f1.waitForFinished();
	f2.waitForFinished();
	f3.waitForFinished();
	f4.waitForFinished();

	return size * 3;
  }
#else // QTB_MULTI_THREAD_CONVERTER
  // convert YUV420 to RGB24
  return convertYUV420toRGB24();
#endif // QTB_MULTI_THREAD_CONVERTER
}

#if QTB_MULTI_THREAD_CONVERTER

#define YUV420TORGB24_MT_VERSION 0

// 5     (2 threads) - 192 fps
// other (2 threads) - 175 fps
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
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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

	  rgb24size += 12;
	}
	rgb24top += qtbrynhildr::rgb24Next;
	if (yPos & 0x1){
	  utop += qtbrynhildr::uvNext;
	  vtop += qtbrynhildr::uvNext;
	}
  }
  return rgb24size;
}

#else // YUV420TORGB24_MT_VERSION == 5

// YUV420 convert to RGB macro
#define GET_R_MT(Y, V)		((256 * Y           + 358 * V) >> 8)
#define GET_G_MT(Y, U, V)	((256 * Y -  88 * U - 182 * V) >> 8)
#define GET_B_MT(Y, U)		((256 * Y + 453 * U          ) >> 8)


// qtbrynhhildr::convertYUV420toRGB24() (NOT GraphicsThread::convertYUV420toRGB24())
int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height)
{
  int rgb24size = 0;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < qtbrynhildr::width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;

	  // == xPos ==
	  y = *ytop++;

	  // R
	  r = qtbrynhildr::clip(GET_R_MT(y, v));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_G_MT(y, u, v));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_B_MT(y, u));
	  *rgb24top++ = (uchar)b;

	  // == xPos+1 ==
	  y = *ytop++;

	  // R
	  r = qtbrynhildr::clip(GET_R_MT(y, v));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = qtbrynhildr::clip(GET_G_MT(y, u, v));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = qtbrynhildr::clip(GET_B_MT(y, u));
	  *rgb24top++ = (uchar)b;

	  rgb24size += 6;
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

#endif  // QTB_MULTI_THREAD_CONVERTER

// yuv420toRGB24 (select one)
// =========================================================
// YUV420TORGB24_VERSION 1 : float                   84 fps
// YUV420TORGB24_VERSION 2 : integer and shift      129 fps
// YUV420TORGB24_VERSION 3 : V2 + small improvement 131 fps
// YUV420TORGB24_VERSION 4 : V3 + small improvement 132 fps
// YUV420TORGB24_VERSION 5 : V4 + loop unrolling    136 fps
// =========================================================
#define YUV420TORGB24_VERSION 5


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
int GraphicsThread::convertYUV420toRGB24()
{
  int rgb24size = 0;
  uchar *rgb24top = rgb24;
  uchar *ytop = ytopOrg;
  uchar *utop = utopOrg;
  uchar *vtop = vtopOrg;

  // last line top
  rgb24top += width * (height - 1) * 3;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;

	  // == xPos ==
	  y = *ytop++;

	  // R
	  r = clip(GET_R(y, v));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb24top++ = (uchar)b;

	  // == xPos+1 ==
	  y = *ytop++;

	  // R
	  r = clip(GET_R(y, v));
	  *rgb24top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb24top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb24top++ = (uchar)b;

	  rgb24size += 6;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
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
int GraphicsThread::convertYUV420toRGB24()
{
  int rgb24size = 0;
  uchar *rgb24top = rgb24;
  uchar *ytop = ytopOrg;
  uchar *utop = utopOrg;
  uchar *vtop = vtopOrg;

  // last line top
  rgb24top += width * (height - 1) * 3;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;

	  // set u/v
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;

	  // == xPos ==
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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

	  rgb24size += 6;
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
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
int GraphicsThread::convertYUV420toRGB24()
{
  uchar *rgb24top = rgb24;
  int rgb24size = 0;
  uchar *ytop = ytopOrg;
  uchar *utop = utopOrg;
  uchar *vtop = vtopOrg;

  // last line top
  rgb24top += width * (height - 1) * 3;

  for (int yPos = 0; yPos < height; yPos++){
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
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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

	  rgb24size += 6;
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
int GraphicsThread::convertYUV420toRGB24()
{
  uchar *rgb24top = rgb24;
  int rgb24size = 0;
  uchar *ytop = ytopOrg;
  uchar *utop = utopOrg;
  uchar *vtop = vtopOrg;

  // last line top
  rgb24top += width * (height - 1) * 3;

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
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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
	  y = *ytop++;
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

	  // == xPos+1 ==
	  y = *ytop++;
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

	  rgb24size += 12;
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

#endif // QTB_PUBLIC_MODE7_SUPPORT

} // end of namespace qtbrynhildr
