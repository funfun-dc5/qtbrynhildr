// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>

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

  // sample rate
  SAMPLERATE samplerate;

  // audio output
  QAudioOutput *audioOutput;

  // IO device
  QIODevice *output;

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

  // get sound buffer
  SoundBuffer *getSoundBuffer() const
  {
	return soundBuffer;
  }

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
  // put PCM data into sound device
  TRANSMIT_RESULT putPCMDataIntoSoundDevice();

  // change samplerate
  bool changeSamplerate(SAMPLERATE samplerate);

  // create .wav file
  void createWavFile(int dataSize);

#if defined(DEBUG)
private slots:
  // stateCanged
  void handleStateChanged(QAudio::State state);
#endif // defined(DEBUG)
};

} // end of namespace qtbrynhildr

#endif // SOUNDTHREAD_H
