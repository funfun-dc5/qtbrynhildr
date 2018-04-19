// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef GRAPHICSBUFFER_H
#define GRAPHICSBUFFER_H
// Common Header
#include "common/common.h"

// System Header
#if QTB_REC
#include <fstream>
#include <iostream>
#endif // QTB_REC

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// GraphicsBuffer
class GraphicsBuffer
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
public:
  typedef enum {
	TYPE_JPEG,
	TYPE_VP8
  } FrameType;

  // frame table size
  static const int FRAME_TABLE_NUM = 256;

private:
  // frame entry
  typedef struct {
	FrameType type;	// data type for this frame
	unsigned int rate;	// data frame rate for this frame
	int	size;		// data size for this frame
  } FrameEntry;

  // ring buffer
  RingBuffer *ringBuffer;

  // frame table
  FrameEntry frameTable[FRAME_TABLE_NUM];

  // current frame no
  int currentFrameNo;

  // next frame no
  int nextFrameNo;

  // current frame count
  int frameCount;

  // output log flag
  bool outputLog;

#if QTB_REC
  fstream *file;
#endif // QTB_REC

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  GraphicsBuffer(int size);
  // destructor
  ~GraphicsBuffer();

  // clear buffer
  void clear();

  // put frame
  int putFrame(const char *buf, int len, FrameType type, unsigned int rate);

  // get frame
  int getFrame(char *buf, FrameType *type, unsigned int *rate);

  // get frame count
  int getFrameCount() const
  {
	return frameCount;
  }

  // get size
  int getSize() const;
};

} // end of namespace qtbrynhildr

#endif // GRAPHICSBUFFER_H
