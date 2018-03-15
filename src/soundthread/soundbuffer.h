// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QIODevice>

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// SoundBuffer
class SoundBuffer : public RingBuffer, public QIODevice
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  SoundBuffer(int size);
  // destructor
  ~SoundBuffer();

protected:
  // QIODevice interface
  qint64 readData(char *data, qint64 maxlen);
  qint64 writeData(const char *data, qint64 len);
  qint64 bytesAvailable() const;

private:

};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
