// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H
// Common Header
#include "common/common.h"

// Qt Header
#if QTB_NEWFEATURE_SB
#include <QIODevice>
#endif // QTB_NEWFEATURE_SB

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// SoundBuffer
#if QTB_NEWFEATURE_SB
class SoundBuffer : public RingBuffer, public QIODevice
#else // QTB_NEWFEATURE_SB
class SoundBuffer : public RingBuffer
#endif // QTB_NEWFEATURE_SB
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
#if QTB_NEWFEATURE_SB
  // QIODevice interface
  qint64 readData(char *data, qint64 maxlen);
  qint64 writeData(const char *data, qint64 len);
  qint64 bytesAvailable() const;
#endif // QTB_NEWFEATURE_SB

private:

};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
