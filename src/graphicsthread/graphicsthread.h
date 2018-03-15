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
#if QTB_NEWFEATURE_GB
#include "graphicsbuffer.h"
#endif // QTB_NEWFEATURE_GB

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

#if QTB_MULTI_THREAD_CONVERTER
// parameters
extern int width;
extern int uvNext;
extern int rgb24Next;

extern uchar *yuv420;
extern uchar *yuv1;
extern uchar *y1topOrg;
extern uchar *u1topOrg;
extern uchar *v1topOrg;
extern uchar *y2topOrg;
extern uchar *u2topOrg;
extern uchar *v2topOrg;
#endif // QTB_MULTI_THREAD_CONVERTER

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // desktop panel
  DesktopPanel *desktopPanel;

  // image for desktop
  QImage *image;

#if QTB_NEWFEATURE_GB
  // graphics buffer
  GraphicsBuffer *graphicsBuffer;
#endif // QTB_NEWFEATURE_GB

  // desktop scaling factor
  qreal desktopScalingFactor;

  // check counter
  int checkCounter;

  // frame counter
  unsigned int frameCounter;

  // previous get frame rate time
  qint64 previousGetFrameRateTime;

#if !QTB_NEWFEATURE_GB
  // start draw frame time
  qint64 startDrawFrameTime;

  // average draw frame time
  qint64 averageDrawFrameTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // draw time (MODE5/6: JPEG, MODE7: YUV->RGB and RGB888)
  qint64 drawTime;

  // start draw time
  qint64 startDrawTime;
#else // !QTB_NEWFEATURE_GB
  // total frame counter
  unsigned int totalFrameCounter;
#endif // !QTB_NEWFEATURE_GB

  // clearDesktop
  bool onClearDesktop;

#if QTB_PUBLIC_MODE7_SUPPORT
  // width of image
  int width;

  // height of image
  int height;

  // buffer for yuv420
  uchar *yuv1; // buffer 1
  uchar *yuv2; // buffer 2

  uchar *yuv420; // current yuv buffer

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
  uchar *y1topOrg;
  uchar *u1topOrg;
  uchar *v1topOrg;
  uchar *y2topOrg;
  uchar *u2topOrg;
  uchar *v2topOrg;

  int size; // width * height
  int uvNext;
  int rgb24Next;

  // codec context
  vpx_codec_ctx_t c_codec;

  // SIMD instruction
  bool hasSIMDInstruction;
#endif // QTB_PUBLIC_MODE7_SUPPORT

  // local buffer
  char *buffer;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
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

#if !QTB_NEWFEATURE_GB
  // reset parameters
  void resetDrawParamaters()
  {
	averageDrawFrameTime = 0;
	drawTime = 0;
	checkCounter = 0;
  }
#endif // !QTB_NEWFEATURE_GB

#if QTB_NEWFEATURE_GB
  // get graphics buffer
  GraphicsBuffer *getGraphicsBuffer() const
  {
	return graphicsBuffer;
  }
#endif // QTB_NEWFEATURE_GB

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
  void convertYUV420toRGB24(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);

  // get converter source name
  const char *getConverterSourceName() const;

#if QTB_SIMD_SUPPORT
  // get converter source name for SIMD operation
  const char *getConverterSourceName_SIMD() const;

  // make RGB24 image by SIMD operation
  inline int makeRGB24Image_SIMD();

  // convert YUV420 to RGB24 (Full Convert)
  void convertYUV420toRGB24_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgb24top, int height);
#endif // QTB_SIMD_SUPPORT

  // clip
  int clip(int val)
  {
	if (val < 0) return 0;
	if (val > 255) return 255;
	return val;
  }
#endif // QTB_PUBLIC_MODE7_SUPPORT

signals:
  // desktop Changed
  void desktopChanged(QImage image);

  // desktop Clear
  void desktopClear();
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
