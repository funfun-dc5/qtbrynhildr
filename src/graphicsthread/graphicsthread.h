// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QImage>

// Local Header
#include "common/netthread.h"

// count for draw time check
#define DRAW_TIME_SAMPLING_POINT 20

#if QTB_PUBLIC_MODE7_SUPPORT
// libvxp Header
#include "vpx_decoder.h"
#include "vp8dx.h"

// use loadFromData() instead of new QImage()
#define USE_PPM_LOADER_FOR_VP8	0

#if USE_PPM_LOADER_FOR_VP8
// for PPM Header
#define PPM_HEADER_SIZE_MAX 32
// "P6\n"            : PPM binary
// "wwww hhhh\n"     : width height
// "255\n"           : max value (255)
#define PPM_HEADER_FORMAT "P6\n%d %d\n255\n"
#endif // USE_PPM_LOADER_FOR_VP8

// RGB image format
#if USE_PPM_LOADER_FOR_VP8
#define FORMAT_RGB888 1 // fixed
#else // USE_PPM_LOADER_FOR_VP8
#define FORMAT_RGB888 1
//#define FORMAT_RGBA8888 1
#endif // USE_PPM_LOADER_FOR_VP8

// RGB888 (3bytes) format info.
#if FORMAT_RGB888
#define IMAGE_FORMAT QImage::Format_RGB888
#define IMAGE_FORMAT_SIZE 3
#endif // FORMAT_RGB888

// RGBA8888 (4bytes) format info.
#if FORMAT_RGBA8888
#define IMAGE_FORMAT QImage::Format_RGBA8888
#define IMAGE_FORMAT_SIZE 4
#endif // FORMAT_RGBA8888

#endif // QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

public:
  // constructor
  GraphicsThread(Settings *settings, DesktopPanel *desktopPanel = 0);
  // destructor
  ~GraphicsThread();

  // get frame rate
  double getFrameRate();

  // get total frame counter
  unsigned int getTotalFrameCounter() const
  {
	return totalFrameCounter;
  }

protected:
  // connect to server
  CONNECT_RESULT connectToServer();

  // process for header
  PROCESS_RESULT processForHeader();

  // transmit local buffer to global buffer
  TRANSMIT_RESULT transmitBuffer();

  // connected
  void connectedToServer();

  // shutdown connection
  void shutdownConnection();

#if QTB_PUBLIC_MODE7_SUPPORT
private:
  // setup for yuv420, rgb24
  inline bool setup();

  // make YUV420 image
  inline bool makeYUV420Image();

  // make RGB24 image
  inline int makeRGB24Image();

  // convert YUV420 to RGB24 (Full Convert)
  int convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);

  // clip
  int clip(int val)
  {
	if (val < 0) return 0;
	if (val > 255) return 255;
	return val;
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

private:
  // image for desktop
  QImage *image;

  // frame counter
  unsigned int frameCounter;

  // previous get frame rate time
  QDateTime previousGetFrameRateTime;

  // start draw frame time
  QDateTime startDrawFrameTime;

  // average draw frame time
  qint64 averageDrawFrameTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // draw time (MODE5/6: JPEG, MODE7: YUV->RGB and RGB888)
  qint64 drawTime;

  // start draw time
  QDateTime startDrawTime;

  // clearDesktop
  bool onClearDesktop;

#if QTB_PUBLIC_MODE7_SUPPORT
  // width of image
  int width;

  // height of image
  int height;

  // buffer for yuv420
  uchar *yuv420;

#if USE_PPM_LOADER_FOR_VP8
  // buffer for ppm
  uchar *ppm;
#endif // USE_PPM_LOADER_FOR_VP8

  // buffer for rgb24
  uchar *rgb24;

  // vpx init flag
  bool doneVpxInit;

  // parameters for decodeVP8()
  int hwidth;

  // parameters for convertYUV420toRGB24()
  uchar *ytopOrg;
  uchar *utopOrg;
  uchar *vtopOrg;

  int size; // width * height
  int uvNext;
  int rgb24Next;

  // codec context
  vpx_codec_ctx_t c_codec;

#endif // QTB_PUBLIC_MODE7_SUPPORT

private:
  // local buffer
  char *buffer;

signals:
  // desktop Changed
  void desktopChanged(QImage image);

  // desktop Clear
  void desktopClear();
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
