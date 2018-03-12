// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef RINGBUFFER_H
#define RINGBUFFER_H
// Common Header
#include "common/common.h"

// Qt Header

namespace qtbrynhildr {

// RingBuffer
class RingBuffer
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // local buffer
  char *buffer;

  // local buffer for copying 2 region
  char *pairBuffer;

  // buffer size
  int bufferSize;

  // buffer top index
  int topPos;

  // buffer next index
  int nextPos;

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  RingBuffer(int size);
  // destructor
  ~RingBuffer();

  // put data to ring buffer
  int put(const char *buf, int len);

  // get data from ring buffer
  const char *get(int len);

  // clear buffer
  void clear();

  // get available data size
  long size() const;

  // get available data size (max size = len)
  long size(int len) const;
};

} // end of namespace qtbrynhildr

#endif // RINGBUFFER_H
