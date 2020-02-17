// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "common/netthread.h"
#if 1 // for TEST
#include "decoder.h"
#include "decoder_jpeg.h"
#include "decoder_vp8_cpp.h"
#include "decoder_vp8_sse.h"
#include "decoder_vp8_avx2.h"
//#include "decoder_vp8_neon.h"
#endif // 1 // for TEST
#include "framecontroler.h"
#include "framecounter.h"
#if QTB_TEST_CODE
#include "graphicsbuffer.h"
#endif // QTB_TEST_CODE

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
  // graphics buffer
  GraphicsBuffer *graphicsBuffer;

  // graphics buffer size
  int graphicsBufferSize;
#endif // QTB_TEST_CODE

  // frame counter
  FrameCounter frameCounter;

  // drawing flag
  bool onDrawing;

#if !QTB_TEST_CODE

#if 1 // for TEST
  // image for drawing desktop
  QImage *image;
#endif // 0 // for TEST

  // clear desktop flag
  bool onClearDesktop;

#if QTB_SIMD_SUPPORT
  // has SIMD instruction
  bool hasSIMDInstruction;
#endif // QTB_SIMD_SUPPORT
#endif // !QTB_TEST_CODE

  // local buffer
  char *buffer;

#if QTB_BENCHMARK
  int initialBenchmarkPhaseCounter;
  int benchmarkPhaseCounter;
#endif // QTB_BENCHMARK

  // frame controler
  FrameControler frameControler;

#if 1 // for TEST
  // decoders
  Decoder *decoderMode56;	// for MODE5/6
  Decoder *decoderMode7;	// for MODE7
  Decoder *decoderMode7SIMD;// for MODE7(SIMD)

  // current decoder
  Decoder *decoder;

  // current video mode
  VIDEO_MODE video_mode;
#endif // 1 // for TEST

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

#if !QTB_TEST_CODE
#if 0 // for TEST
  // draw graphics
  void draw_Graphics(int size);
  // draw graphics MJPEG
  inline bool draw_Graphics_MJPEG(int size);
  // draw graphics COMPRESS
  inline bool draw_Graphics_COMPRESS(int size);
#endif // 0 // for TEST
#endif // !QTB_TEST_CODE

  // output received data
  void outputReceivedData(long receivedDataSize);

#if !QTB_TEST_CODE
signals:
  // draw desktop
  void drawDesktop(QImage image);

  // clear desktop
  void clearDesktop();
#endif // !QTB_TEST_CODE
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSTHREAD_H
