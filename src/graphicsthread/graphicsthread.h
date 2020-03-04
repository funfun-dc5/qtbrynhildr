// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "common/netthread.h"
#include "decoder.h"
#include "framecontroller.h"
#include "framecounter.h"
#include "graphicsbuffer.h"

namespace qtbrynhildr {

// GraphicsThread
class GraphicsThread : public NetThread
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
#if QTB_TEST_CODE
  // graphics buffer size
  int graphicsBufferSize;

  // graphics buffer
  GraphicsBuffer *graphicsBuffer;
#endif // QTB_TEST_CODE

  // frame counter
  FrameCounter frameCounter;

  // drawing flag
  bool onDrawing;

  // image for drawing desktop
  QImage *image;

  // clear desktop flag
  bool onClearDesktop;

#if QTB_SIMD_SUPPORT
  // has SIMD instruction
  bool hasSIMDInstruction;
#endif // QTB_SIMD_SUPPORT

  // local buffer
  char *buffer;

#if QTB_BENCHMARK
  int initialBenchmarkPhaseCounter;
  int benchmarkPhaseCounter;
#endif // QTB_BENCHMARK

  // frame controller
  FrameController frameController;

  // decoders
  Decoder *decoderMode56;	// for MODE5/6
  Decoder *decoderMode7;	// for MODE7
#if QTB_SIMD_SUPPORT
  Decoder *decoderMode7SIMD;// for MODE7(SIMD)
#endif // QTB_SIMD_SUPPORT

  // current decoder
  Decoder *decoder;

  // current video mode
  VIDEO_MODE video_mode;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
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

#if QTB_TEST_CODE
  // get graphics buffer
  GraphicsBuffer *getGraphicsBuffer() const
  {
	return graphicsBuffer;
  }
#endif // QTB_TEST_CODE

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

#if QTB_BENCHMARK
  void setInitialBenchmarkPhaseCounter(int initialBenchmarkPhaseCounter)
  {
	this->initialBenchmarkPhaseCounter = initialBenchmarkPhaseCounter;
  }
#endif // QTB_BENCHMARK

#if QTB_SIMD_SUPPORT
  // get SIMD decoder name
  const char* getSIMDDecoderName()
  {
	if (decoderMode7SIMD != nullptr){
	  return decoderMode7SIMD->name();
	}
	else {
	  return nullptr;
	}
  }
#endif // QTB_SIMD_SUPPORT

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

signals:
  // draw desktop
  void drawDesktop(QImage image);

  // clear desktop
  void clearDesktop();
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
