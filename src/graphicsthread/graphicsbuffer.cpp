// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Local Header
#define QTB_REC 0
#include "graphicsbuffer.h"

namespace qtbrynhildr {

// constructor
GraphicsBuffer::GraphicsBuffer(int size)
  :ringBuffer(0)
  ,currentFrameNo(0)
  ,nextFrameNo(0)
  ,frameCount(0)
  // for DEBUG
  ,outputLog(false)
{
  // initialize
  ringBuffer = new RingBuffer(size);
  memset(frameTable, 0, sizeof(frameTable));

#if QTB_REC
  file = new fstream;
  file->open("frame_data.fdx", ios::out | ios::binary | ios::trunc);
#endif // QTB_REC
}

// destructor
GraphicsBuffer::~GraphicsBuffer()
{
#if QTB_REC
  if (file->is_open()){
	file->close();
  }
#endif // QTB_REC

  // delete objects
  if (ringBuffer != 0){
	delete ringBuffer;
	ringBuffer = 0;
  }
}

// clear buffer
void GraphicsBuffer::clear()
{
  // clear frame table
  for(int i = 0; i < FRAME_TABLE_NUM; i++){
	frameTable[i].size = 0;
  }
  ringBuffer->clear();
}

// put frame
bool GraphicsBuffer::putFrame(const char *buf, int len)
{
  // check table
  if (frameCount >= FRAME_TABLE_NUM){
	return false;
  }

  // copy to ring buffer from buf
  int result = ringBuffer->put(buf, len);
  if (result != len){
	return false;
  }

#if QTB_REC
  // len (4 bytes) + frame data (len bytes) + len (4 bytes) + ...
  file->write((char*)&len, sizeof(len));
  file->write(buf, len);
#endif // QTB_REC

  // set to frame table
  frameTable[nextFrameNo].size = len;
  nextFrameNo = (nextFrameNo >= FRAME_TABLE_NUM - 1) ? 0 : nextFrameNo + 1;

  // current frame count++
  frameCount++;

  //cout << "ring buffer size = " << ringBuffer->getSize() << endl << flush; // for TEST

  return true;
}

// get frame
int GraphicsBuffer::getFrame(char *buf)
{
  // check table
  if (frameCount <= 0){
	return 0;
  }

  // copy to buf from ring buffer
  int len = frameTable[currentFrameNo].size;
  const char *result = ringBuffer->get(len);
  if (result == 0){
	return 0;
  }
  memcpy(buf, result, len);

  // clear frame table entry
  frameTable[currentFrameNo].size = 0;
  currentFrameNo = (currentFrameNo >= FRAME_TABLE_NUM - 1) ? 0 : currentFrameNo + 1;

  // current frame count--
  frameCount--;

  return len;
}

} // end of namespace qtbrynhildr
