// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

// for TEST
#define HAS_QIODEVICE 0

// Common Header
#include "common/common.h"

// Qt Header
#if HAS_QIODEVICE
#include <QIODevice>
#endif // HAS_QIODEVICE

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// SoundBuffer
#if HAS_QIODEVICE
class SoundBuffer : public RingBuffer, public QIODevice
#else // HAS_QIODEVICE
class SoundBuffer : public RingBuffer
#endif // HAS_QIODEVICE
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

#if HAS_QIODEVICE
protected:
  // QIODevice interface
  qint64 readData(char *data, qint64 maxlen);
  qint64 writeData(const char *data, qint64 len);
  qint64 bytesAvailable() const;
#endif // HAS_QIODEVICE

private:

};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
