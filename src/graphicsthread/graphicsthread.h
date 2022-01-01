// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QImage>
#include <QMap>
#include <QSize>
#include <QStringList>

// Local Header
#include "common/netthread.h"
#include "decoder.h"
#include "framecounter.h"

namespace qtbrynhildr {

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // frame counter
  FrameCounter frameCounter;

  // drawing flag
  bool onDrawing;

  // image for drawing desktop
  //QImage *image;

  // desktop size
  QSize currentSize;

  // previous desktop size
  QSize previousSize;

#if QTB_SIMD_SUPPORT
  // has SIMD instruction
  bool hasSIMDInstruction;
#endif // QTB_SIMD_SUPPORT

  // local buffer
  char *buffer;

  // decoders
  Decoder *decoderMode56;	// for MODE5/6
  Decoder *decoderMode7;	// for MODE7
  QMap <QString, Decoder*>decoderMode7Map;
  QStringList decoderMode7NameList;

  // current decoder
  Decoder *decoder;

  // current video mode
  VIDEO_MODE video_mode;

#if QTB_BENCHMARK
  int initialBenchmarkPhaseCounter;
  int benchmarkPhaseCounter;
#endif // QTB_BENCHMARK

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // utilities
  inline static int calcWidthForMode7(int width)
  {
	return (width + 3) & ~3;
  }
  inline static int calcHeightForMode7(int height)
  {
	return (height + 3) & ~3;
  }
  inline static int calcWidth(int width)
  {
	return width;
  }
  inline static int calcHeight(int height)
  {
	return height;
  }

public:
  // constructor
  GraphicsThread(Settings *settings);
  // destructor
  ~GraphicsThread();

  // get frame rate
  double getFrameRate()
  {
	return frameCounter.getFrameRate();
  }

  // get total frame counter
  unsigned int getTotalFrameCounter() const
  {
	return frameCounter.getTotalFrameCounter();
  }

  // set drawing flag
  void setOnDrawing(bool onDrawing)
  {
	this->onDrawing = onDrawing;
  }

  // get drawing flag
  bool getOnDrawing() const
  {
	return onDrawing;
  }

#if QTB_SIMD_SUPPORT
  // get SIMD decoder name list
  QStringList getSIMDDecoderNameList()
  {
	return decoderMode7NameList;
  }
#endif // QTB_SIMD_SUPPORT

#if QTB_BENCHMARK
  void setInitialBenchmarkPhaseCounter(int initialBenchmarkPhaseCounter)
  {
	this->initialBenchmarkPhaseCounter = initialBenchmarkPhaseCounter;
  }
#endif // QTB_BENCHMARK

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

private:
  // receive data
  inline long receiveData(char *buf, long size);

  // output received data
  void outputReceivedData(long receivedDataSize);

  // draw desktop image
  inline void drawDesktopImage(char *buf, int size, VIDEO_MODE mode);

  // rescale image
  inline void rescaleDesktopImage(QImage *image);

  // get desktop scaling factor
  qreal getDesktopScalingFactor(QSize targetSize);

  // adjust frame
  //inline void adjustFrame();

  // check skip frame
  inline bool doSkipFrame(char frame_no);

  // get width for current MODE
  inline SIZE getWidthForCurrentMode(SIZE width)
  {
	if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
	  return (width + 3) & ~3;
	}
	else {
	  return width;
	}
  }

  // get height for current MODE
  inline SIZE getHeightForCurrentMode(SIZE height)
  {
	if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
	  return (height + 3) & ~3;
	}
	else {
	  return height;
	}
  }

  // get size for current MODE
  inline QSize getSizeForCurrentMode(QSize size)
  {
	if (settings->getPublicModeVersion() == PUBLICMODE_VERSION7){
	  return QSize(getWidthForCurrentMode(size.width()),
				   getHeightForCurrentMode(size.height()));
	}
	else {
	  return size;
	}
  }

signals:
  // draw desktop
  void drawDesktop(QImage image);

  // clear desktop
  void clearDesktop();
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
