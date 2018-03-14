// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Local Header
#include "soundbuffer.h"

namespace qtbrynhildr {

// constructor
SoundBuffer::SoundBuffer(int size)
  :RingBuffer(size)
{
}

// destructor
SoundBuffer::~SoundBuffer()
{
}

// QIODevice interface
qint64 SoundBuffer::readData(char *data, qint64 maxlen)
{
  return 0; // for TEST
}

qint64 SoundBuffer::writeData(const char *data, qint64 len)
{
  Q_UNUSED(data);
  Q_UNUSED(len);

  return 0;
}

qint64 SoundBuffer::bytesAvailable() const
{
  return 0; // for TEST
}

} // end of namespace qtbrynhildr
