// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// soundthread.h

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

public:
  // constructor
  SoundThread(Settings *settings, MainWindow *mainWindow = 0);
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

private:
  // sound buffer
  SoundBuffer *soundBuffer;

  // sound buffer size
  int soundBufferSize;

  // sample rate
  SAMPLERATE samplerate;

  // audio format
  QAudioFormat *format;

  // audio output
  QAudioOutput *audioOutput;

  // IO device
  QIODevice *output;

#ifdef DEBUG
private slots:
  // stateCanged
  void handleStateChanged(QAudio::State state);
#endif

  // samplerate change count
  int samplerateChangeCount;

#if QTB_CELT_SUPPORT
  // converter
  Converter *converter;
#endif //QTB_CELT_SUPPORT

private:
  // local buffer
  char *buffer;
};

} // end of namespace qtbrynhildr

#endif // SOUNDTHREAD_H
