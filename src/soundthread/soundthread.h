// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#if !QTB_NEWFEATURE_SB
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#endif // !QTB_NEWFEATURE_SB

// Local Header
#include "common/netthread.h"
#include "soundbuffer.h"

#if QTB_CELT_SUPPORT
#include "converter.h"
#endif //QTB_CELT_SUPPORT

namespace qtbrynhildr {

// SoundThread
class SoundThread : public NetThread
{
  Q_OBJECT

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // sound buffer
  SoundBuffer *soundBuffer;

  // sound buffer size
  int soundBufferSize;

  // sound cache time (ms)
  int soundCacheTime;

  // sample rate
  SAMPLERATE samplerate;

#if !QTB_NEWFEATURE_SB
  // audio format
  QAudioFormat format;

  // audio output
  QAudioOutput *audioOutput;

  // IO device
  QIODevice *output;
#endif // !QTB_NEWFEATURE_SB

  // samplerate change counter
  int samplerateChangeCounter;

#if QTB_CELT_SUPPORT
  // converter
  Converter *converter;
#endif //QTB_CELT_SUPPORT

  // local buffer
  char *buffer;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  SoundThread(Settings *settings);
  // destructor
  ~SoundThread();

protected:
  // connect to server
  CONNECT_RESULT connectToServer();

  // process for header
  PROCESS_RESULT processForHeader();

  // transmit local buffer to global buffer
  TRANSMIT_RESULT transmitBuffer();

  // connected
  void connectedToServer();

  // shutdown connection
  void shutdownConnection();

private:
  // change samplerate
  bool changeSamplerate(SAMPLERATE samplerate);

  // create .wav file
  void createWavFile(int dataSize);

#if !QTB_NEWFEATURE_SB
#if defined(DEBUG)
private slots:
  // stateCanged
  void handleStateChanged(QAudio::State state);
#endif // defined(DEBUG)
#endif // !QTB_NEWFEATURE_SB
};

} // end of namespace qtbrynhildr

#endif // SOUNDTHREAD_H
