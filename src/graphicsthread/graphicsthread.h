// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSTHREAD_H
#define GRAPHICSTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header

// Local Header
#include "common/netthread.h"
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
  unsigned int frameCounter;

  // previous get frame rate time
  qint64 previousGetFrameRateTime;

  // total frame counter
  unsigned int totalFrameCounter;

  // drawing flag
  bool onDrawing;

#if !QTB_TEST_CODE
  // image for drawing desktop
  QImage *image;

  // clear desktop flag
  bool onClearDesktop;

#if QTB_SIMD_SUPPORT
  // has SIMD instruction
  bool hasSIMDInstruction;
#endif // QTB_SIMD_SUPPORT
#endif // !QTB_TEST_CODE

  // local buffer
  char *buffer;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  GraphicsThread(Settings *settings);
  // destructor
  ~GraphicsThread();

  // get frame rate
  double getFrameRate();

  // get total frame counter
  unsigned int getTotalFrameCounter() const
  {
	return totalFrameCounter;
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
  // draw graphics
  void draw_Graphics(int size);
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
