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
  width(0),
  height(0),
  yuv420(0),
  rgb32(0),
  buffer(0)
{
  outputLog = false; // for DEBUG

  // local buffer
  buffer = new char [QTB_GRAPHICS_LOCAL_BUFFER_SIZE];

  // create image
  image = new QImage();

  // initialize libvpx
  if (settings->getPublicModeVersion() >= PUBLICMODE_VERSION7){
	vpx_codec_dec_init(&c_codec, &vpx_codec_vp8_dx_algo, 0, 0);
  }
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

  // buffer for yuv420/rgb32
  if (yuv420 != 0){
	delete [] yuv420;
	yuv420 = 0;
  }
  if (rgb32 != 0){
	delete [] rgb32;
	rgb32 = 0;
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
  if (com_data->video_mode != VIDEO_MODE_MJPEG &&
	  com_data->video_mode != VIDEO_MODE_COMPRESS ){
	return PROCESS_VIDEO_MODE_ERROR;
  }

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
	else if (com_data->video_mode == VIDEO_MODE_COMPRESS ){
	  // VP8
	  uchar *rgb32image = decodeVP8(receivedDataSize);
	  if (rgb32image != 0){
		// create QImage from RGB32
		delete image;
		//image = new QImage(rgb32image, width, height, QImage::Format_RGBA8888);
		image = new QImage(rgb32image, width, height, QImage::Format_RGB888);
		//*image = image->mirrored(false, true);
	  }
	  else {
		if (image->isNull()){
		  delete image;
		  image = new QImage(width, height, QImage::Format_RGBA8888);
		  image->fill(Qt::gray);
		}
	  }

	  desktopLoadResult = true;
	}
	else {
	  // illegal VIDEO_MODE
	}

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

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// decode VP8
uchar *GraphicsThread::decodeVP8(int size)
{
  // decode vp8
  vpx_codec_decode(&c_codec, (uint8_t*)buffer, size, 0, 0);

  // get 1 frame image
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

	// allocate yuv420/rgb32 buffer
	if (yuv420 != 0){
	  delete [] yuv420;
	}
	yuv420 = new uchar[width*height + width*height/2];
	if (rgb32 != 0){
	  delete [] rgb32;
	}
	rgb32 = new uchar[width*height*4];
	//rgb32 = new uchar[width*height*3];

#if 1 // for TEST
	// calc parameters
	hwidth = width / 2;
	size = width * height;
	ytopOrg = (uchar*)yuv420;
	utopOrg = ytopOrg + size;
	vtopOrg = utopOrg + size / 4;
	uvNext = width / 2;
	//rgb32Prev = width * 4 * 2;
	rgb32Prev = width * 3 * 2;
#endif // for TEST
  }

  // create yuv420
  uchar *top = yuv420;
#if 1 // for TEST
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
#else // for TEST
  // Y
  uchar *buf = img->planes[0];
  int stride = img->stride[0];
  int next = width;
  for(int yPos = 0; yPos < height; yPos++){
	memcpy(top, buf, next);
	top += next;
	buf += stride;
  }
  // U
  buf = img->planes[1];
  stride = img->stride[1];
  next = width/2;
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, next);
	top += next;
	buf += stride;
  }
  // V
  buf = img->planes[2];
  stride = img->stride[2];
  next = width/2;
  for(int yPos = 0; yPos < height; yPos += 2){
	memcpy(top, buf, next);
	top += next;
	buf += stride;
  }
#endif // for TEST

  // convert YUV420 to RGB32
  if (convertYUV420toRGB32() != 0){
	return rgb32;
  }
  else {
	return 0;
  }
}

#if 1 // for TEST

#define GET_R(Y, V)		(Y             + 1.402 * V)
#define GET_G(Y, U, V)	(Y - 0.344 * U - 0.714 * V)
#define GET_B(Y, U)		(Y + 1.772 * U            )

//#define GET_R(Y, V)		((256 * Y           + 358 * V) >> 8)
//#define GET_G(Y, U, V)	((256 * Y -  88 * U - 182 * V) >> 8)
//#define GET_B(Y, U)		((256 * Y + 453 * U          ) >> 8)

#else // for TEST

#define GET_R(Y, V)		(Y                   + 1.402 * (V-128))
#define GET_G(Y, U, V)	(Y - 0.344 * (U-128) - 0.714 * (V-128))
#define GET_B(Y, U)		(Y + 1.772 * (U-128)                  )

//#define GET_R(Y, V)		((256 * Y                 + 358 * (V-128)) >> 8)
//#define GET_G(Y, U, V)	((256 * Y -  88 * (U-128) - 182 * (V-128)) >> 8)
//#define GET_B(Y, U)		((256 * Y + 453 * (U-128)                ) >> 8)

#endif // for TEST

// convert YUV420 to RGB32
int GraphicsThread::convertYUV420toRGB32()
{
  uchar *rgb32top = rgb32;
  int rgb32size = 0;
#if 1 // for TEST
  uchar *ytop = ytopOrg;
  uchar *utop = utopOrg;
  uchar *vtop = vtopOrg;
#else // for TEST
  int size = width * height;
  uchar *ytop = (uchar*)yuv420;
  uchar *utop = ytop + size;
  uchar *vtop = utop + size / 4;
  int uvNext = width/2;
  int rgb32Prev = width * 4 * 2;
#endif // for TEST

#if 0 // for TEST
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
	  *rgb32top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb32top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb32top++ = (uchar)b;
	  // A
	  //	  *rgb32top++ = 255;

	  // == xPos+1 ==
	  y = *ytop++;

	  // R
	  r = clip(GET_R(y, v));
	  *rgb32top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb32top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb32top++ = (uchar)b;
	  // A
	  //	  *rgb32top++ = 255;

	  //	  rgb32size += 8;
	  rgb32size += 6;
	}
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	}
  }
#else // for TEST
  // last line top
  //rgb32top += width * (height - 1) * 4;
  rgb32top += width * (height - 1) * 3;

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
#if 1 // for TEST
	  int y, u, v;
#else // for TEST
	  uchar y, u, v;
#endif // for TEST

	  // set u/v
#if 1 // for TEST
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;
#else // for TEST
	  u = *(utop + uvOffset);
	  v = *(vtop + uvOffset);
#endif // for TEST

	  // == xPos ==
	  y = *ytop++;

	  // R
	  r = clip(GET_R(y, v));
	  *rgb32top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb32top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb32top++ = (uchar)b;
	  // A
	  //*rgb32top++ = 255;

	  // == xPos+1 ==
	  y = *ytop++;

	  // R
	  r = clip(GET_R(y, v));
	  *rgb32top++ = (uchar)r;
	  // G
	  g = clip(GET_G(y, u, v));
	  *rgb32top++ = (uchar)g;
	  // B
	  b = clip(GET_B(y, u));
	  *rgb32top++ = (uchar)b;
	  // A
	  //*rgb32top++ = 255;

	  //rgb32size += 8;
	  rgb32size += 6;
	}
	rgb32top -= rgb32Prev;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	}
  }
#endif // for TEST
  return rgb32size;
}

} // end of namespace qtbrynhildr
