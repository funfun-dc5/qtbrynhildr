// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// soundbuffer.h

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H
// Common Header
#include "common/common.h"

// Qt Header

namespace qtbrynhildr {

// SoundBuffer
class SoundBuffer
{
public:
  // constructor
  SoundBuffer(int size);
  // destructor
  ~SoundBuffer();

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
};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
