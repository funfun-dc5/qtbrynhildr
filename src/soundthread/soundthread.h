// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

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

  // get sample rate
  SAMPLERATE getSampleRate() const
  {
	return samplerate;
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
  // receive data
  inline long receiveData(char *buf, long size);

  // put PCM data into sound device
  TRANSMIT_RESULT putPCMDataIntoSoundDevice();

  // change samplerate
  bool changeSamplerate(SAMPLERATE samplerate);

  // output received data
  void outputReceivedData(long receivedDataSize, const char *filename);

  // create .wav file
  void createWavFile(int dataSize);

#if 0 // defined(Q_OS_ANDROID)
  // sound data check
  inline bool hasSoundData(char *buf, long len)
  {
	for(int i = 0; i < len; i++){
	  if (buf[i] != 0) return true;
	}
	return false;
  }
#endif // defined(Q_OS_ANDROID)

private slots:
#if defined(DEBUG)
  // stateCanged
  void handleStateChanged(QAudio::State state);
#endif // defined(DEBUG)

#if 0 // for TEST
  void notify();
#endif // 0 // for TEST

signals:
  // connected
  void connected();

  // refresh window
  void refreshWindow();
};

} // end of namespace qtbrynhildr

#endif // SOUNDTHREAD_H
