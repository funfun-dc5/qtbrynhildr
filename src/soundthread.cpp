// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// soundthread.cpp

// Common Header
#include "common.h"

// System Header
#include <fstream>
#include <iostream>

// Qt Header
#include <QAudio>
#include <QAudioDeviceInfo>

// Local Header
#include "soundthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"

namespace qtbrynhildr {

// constructor
SoundThread::SoundThread(Settings *settings, MainWindow *mainWindow)
  :
  NetThread("SoundThread", settings, mainWindow),
  soundBuffer(0),
  soundBufferSize(0),
  format(0),
  audioOutput(0),
  output(0),
  buffer(0)
{
  outputLog = false; // for DEBUG

  // sound buffer
  soundBufferSize = settings->getSoundBufferSize();
  soundBuffer = new SoundBuffer(soundBufferSize);

  // local buffer
  buffer = new char [QTB_SOUND_LOCAL_BUFFER_SIZE];
}

// destructor
SoundThread::~SoundThread()
{
  // delete objects
  // com_data
  if (com_data != 0){
	delete com_data;
	com_data = 0;
  }

  // sound buffer
  if (soundBuffer != 0){
	delete soundBuffer;
	soundBuffer = 0;
  }
  soundBufferSize = 0;

  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }

  // audio output
  if (audioOutput != 0){
	audioOutput->stop();
	delete audioOutput;
	audioOutput = 0;
  }

  // format
  if (format != 0){
	delete format;
	format = 0;
  }
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// connect to server
CONNECT_RESULT SoundThread::connectToServer()
{
  if (sock_sound != INVALID_SOCKET)
	return CONNECT_SUCCEEDED;

  if (counter_graphics < 5){
	return CONNECT_WAITED_COUNT;
  }

  // connect
  sock_sound = socketToServer();
  if (sock_sound == INVALID_SOCKET){
	return CONNECT_FAILED;
  }

  // create objects
  // header
  if (com_data == 0){
	com_data = new COM_DATA;
  }

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT SoundThread::processForHeader()
{
  // receive header
  if (receiveData(sock_sound, (char *)com_data, sizeof(COM_DATA)) != sizeof(COM_DATA)){
	// error
	cout << "[" << name << "]" << " received data size != sizeof(COM_DATA)" << endl << flush; // error
	return PROCESS_NETWORK_ERROR;
  }

  // Nothing to do for sound
  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT SoundThread::transmitBuffer()
{
  // check samplerate
  if (com_data->samplerate != 0 && com_data->samplerate != samplerate){
	bool result = changeSamplerate(com_data->samplerate);
	if (result){
	  // new samplerate
	  samplerate = com_data->samplerate;
	  //	  cout << "samplerate: " << samplerate << endl << flush;
	}
	else {
	  // error
	  // Nothing to do
	}
  }

  // check
  // received data size
  long receivedDataSize = com_data->data_size;
  // size check
  if (receivedDataSize < 0){
	// error
	return TRANSMIT_DATASIZE_ERROR;
  }
  if (receivedDataSize == 0){
	// Nothing to do
	return TRANSMIT_SUCCEEDED;
  }
  if (receivedDataSize > QTB_SOUND_LOCAL_BUFFER_SIZE){
	cout << "[SoundThread] receivedDataSize = " << receivedDataSize << endl << flush; // error
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive PCM raw Data
  receivedDataSize = receiveData(sock_sound, buffer, receivedDataSize);
  // no data
  if (receivedDataSize <= 0){
	// Nothing to do
	return TRANSMIT_DATASIZE_ERROR;
  }

  // buffer[]         : PCM Data
  // receivedDataSize : Size of PCM Data

  // for DEBUG : save PCM Data (append)
  if (settings->getOutputSoundDataToFile()){
	fstream file;
	char filename[QTB_MAXPATHLEN+1];
	snprintf(filename, QTB_MAXPATHLEN, "pcm/%s.jpg", QTB_SOUND_OUTPUT_FILENAME);
	file.open(filename, ios::out | ios::binary | ios::app);
	if (file.is_open()){
	  file.write(buffer, receivedDataSize);
	  file.close();
	}
  }

  // put PCM data into sound buffer
  if (settings->getOnSound()){
	// check audio output
	if (audioOutput == 0){
	  // NOT supported sample rate
	  return TRANSMIT_SUCCEEDED;
	}

	// put into soundBuffer
	int putSize = soundBuffer->put(buffer, receivedDataSize);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into sound buffer
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}

	// write into sound buffer
	if (audioOutput->state() != QAudio::StoppedState){
	  int chunks = audioOutput->bytesFree()/(audioOutput->periodSize());

	  while (chunks){
		qint64 len = soundBuffer->size(audioOutput->periodSize());

		// write PCM data
		if (len != 0){
		  qint64 result = output->write(soundBuffer->get(len), len);
		  if (result != len){
			// Failed to write
			return TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER;
		  }
		}
		else {
		  break;
		}

		// if (len != audioOutput->periodSize())
		//   break;

		--chunks;
	  }
	}

#if 0 // for DEBUG
	static int count = 0;
	if (count % 500 == 0){
	  cout << "[SoundThread] receivedDataSize = " << receivedDataSize << endl;
	  cout << "[SoundThread] size = " << soundBuffer->size() << ", count = " << count << endl;
	  cout << "[SoundThread] bytesFree() =  " << audioOutput->bytesFree() << endl << flush;
	}
	count++;
#endif
  }

  return TRANSMIT_SUCCEEDED;
}

// shutdown connection
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
void SoundThread::shutdownConnection()
{
  if (sock_sound != INVALID_SOCKET){
	shutdown(sock_sound, SD_BOTH);
	closesocket(sock_sound);
	sock_sound = INVALID_SOCKET;
  }
}
#endif // defined(Q_OS_WIN) || defined(Q_OS_LINUX)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// change samplerate
bool SoundThread::changeSamplerate(SAMPLERATE samplerate)
{
  if (settings->getOutputLog()){
	cout << "[SoundThread] changeSamplerate(" << samplerate << ")" << endl << flush;
  }

  if (format == 0){
	format = new QAudioFormat();
  }

  // setting for sound format
  format->setSampleRate((int)samplerate);
  format->setChannelCount(2); // channel
  format->setSampleSize(16); // bits
  format->setCodec("audio/pcm"); // PCM
  format->setByteOrder(QAudioFormat::LittleEndian);
  format->setSampleType(QAudioFormat::SignedInt);

  // audio device information
  const QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
  if (!deviceInfo.isFormatSupported(*format)){
	if (audioOutput != 0){
	  audioOutput->stop();
	  delete audioOutput;
	  audioOutput = 0;
	}
	// NOT supported sample rate!
	emit outputLogMessage(QTB_MSG_NOTSUPPORT_SAMPLE_RATE);
	if (settings->getOutputLog()){
	  QString msg = "sampling rate (" + QString::number(samplerate) + ") is NOT supported.";
	  emit outputLogMessage(PHASE_SOUND, msg);
	}
	return true; // NOT supported sample rate
  }
  // supported Sample Rates
  if (settings->getOutputLog()){
	QList<int> sampleRatesList = deviceInfo.supportedSampleRates();
	qDebug() << "supported Sample Rates : " << sampleRatesList;
  }
  // clean sound buffer
  soundBuffer->clear();

  // create Audio Output
  if (audioOutput != 0){
	audioOutput->stop();
	delete audioOutput;
	audioOutput = 0;
  }
  audioOutput = new QAudioOutput(*format);

  // stateChanged
#if defined(DEBUG)
  connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(handleStateChanged(QAudio::State)));
#endif

  // setting for AudioOutput
  audioOutput->setBufferSize(QTB_AUDIOOUTPUT_SOUND_BUFFER_SIZE);

  // output
  output = audioOutput->start();

  return true;
}

#if defined(DEBUG)
void SoundThread::handleStateChanged(QAudio::State state)
{
    qWarning() << "state = " << state;
}
#endif

} // end of namespace qtbrynhildr
