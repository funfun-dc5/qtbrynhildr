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

#if QTB_SOUND_PULL_MODE
// start iodevice
void SoundBuffer::start()
{
  open(QIODevice::ReadOnly);
}

// stop iodevice
void SoundBuffer::stop()
{
  close();
}

// QIODevice interface
qint64 SoundBuffer::readData(char *data, qint64 maxlen)
{
  qint64 len = getSize(maxlen);

  memcpy(data, get(len), len);

  return len;
}

qint64 SoundBuffer::writeData(const char *data, qint64 len)
{
  Q_UNUSED(data);
  Q_UNUSED(len);

  return 0;
}

qint64 SoundBuffer::bytesAvailable() const
{
  return getSize() + QIODevice::bytesAvailable();
}
#endif // QTB_SOUND_PULL_MODE

} // end of namespace qtbrynhildr
