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

// libvxp Header
#include "vpx_decoder.h"
#include "vp8dx.h"

namespace qtbrynhildr {

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

public:
  // constructor
  GraphicsThread(Settings *settings, MainWindow *mainWindow = 0);
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
  // decode VP8
  uchar *decodeVP8(int size);

  // convert YUV420 to RGB32
  int convertYUV420toRGB32();

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

  // previous frame time
  QDateTime previousFrameTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // clearDesktop
  bool onClearDesktop;

#if QTB_PUBLIC_MODE7_SUPPORT
  // width of image
  int width;

  // height of image
  int height;

  // buffer for yuv420
  uchar *yuv420;

  // buffer for rgb32
  uchar *rgb32;

  // parameters for decodeVP8()
  int hwidth;

  // parameters for convertYUV420toRGB32()
  uchar *ytopOrg;
  uchar *utopOrg;
  uchar *vtopOrg;

  int size; // width * height
  int uvNext;
  int rgb32Prev;

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
