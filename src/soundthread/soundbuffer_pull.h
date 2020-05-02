// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

// Common Header
#include "common/common.h"

// sound pull mode
#define QTB_SOUND_PULL_MODE		1

// Qt Header
#if QTB_SOUND_PULL_MODE
#include <QIODevice>
#endif // QTB_SOUND_PULL_MODE

// Local Header
#include "util/ringbuffer.h"

namespace qtbrynhildr {

// SoundBuffer
#if QTB_SOUND_PULL_MODE
class SoundBuffer : public QIODevice, public RingBuffer
#else // QTB_SOUND_PULL_MODE
class SoundBuffer : public RingBuffer
#endif // QTB_SOUND_PULL_MODE
{
#if QTB_SOUND_PULL_MODE
  Q_OBJECT
#endif // QTB_SOUND_PULL_MODE
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

#if QTB_SOUND_PULL_MODE
public:
  // start iodevice
  void start();
  // stop iodevice
  void stop();

  // QIODevice interface
  qint64 readData(char *data, qint64 maxlen) override;
  qint64 writeData(const char *data, qint64 len) override;
  qint64 bytesAvailable() const override;
#endif // QTB_SOUND_PULL_MODE

private:

};

} // end of namespace qtbrynhildr

#endif // SOUNDBUFFER_H
