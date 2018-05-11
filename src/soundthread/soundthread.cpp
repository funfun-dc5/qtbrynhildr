// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#include <fstream>
#include <iostream>

// Qt Header
#include <QAudio>
#include <QAudioDeviceInfo>

// Local Header
#include "soundthread.h"
#include "parameters.h"
#include "qtbrynhildr.h"

#include "wave.h"

#if QTB_CELT_SUPPORT
#include "converter_celt.h"
#endif //QTB_CELT_SUPPORT

// for TEST
#define TEST_THREAD		0

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoundThread::SoundThread(Settings *settings)
  :NetThread("SoundThread", settings)
  ,soundBuffer(0)
  ,samplerate(0)
  ,audioOutput(0)
  ,output(0)
#if QTB_CELT_SUPPORT
  ,converter(0)
#endif //QTB_CELT_SUPPORT
  ,buffer(0)
{
  outputLog = false; // for DEBUG

  // sound buffer
  soundBuffer = new SoundBuffer(settings->getSoundBufferSize());

  // local buffer
  buffer = new char [QTB_SOUND_LOCAL_BUFFER_SIZE];

  // delete files
  if (settings->getOutputSoundDataToFile()){
	QFile pcmFile("pcm/" QTB_SOUND_OUTPUT_FILENAME);
	pcmFile.remove();
	QFile rawFile("pcm/" QTB_SOUND_OUTPUT_RAW_FILENAME);
	rawFile.remove();
  }
}

// destructor
SoundThread::~SoundThread()
{
  // delete objects
  // sound buffer
  if (soundBuffer != 0){
	delete soundBuffer;
	soundBuffer = 0;
  }

  // local buffer
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }

  // audio output
  if (audioOutput != 0){
	audioOutput->stop();
#if !defined(QTB_RPI3) // for Segmentation Fault on rpi3
	delete audioOutput;
#endif // !defined(QTB_RPI3) // for Segmentation Fault on rpi3
	audioOutput = 0;
  }

  // output wav file
  if (settings->getOutputSoundDataToFile() && settings->getOutputSoundDataToWavFile()){
	QFile pcmFile("pcm/" QTB_SOUND_OUTPUT_FILENAME);
	// create wav file
	int pcmFileSize = pcmFile.size();
	if (pcmFileSize > 0){
	  createWavFile(pcmFileSize);
	}
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

  // connected
  connectedToServer();

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT SoundThread::processForHeader()
{
#if TEST_THREAD
  startTimeInfo();
#endif // TEST_THREAD

  // receive header
  long dataSize;
  dataSize = receiveData((char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got header");
#endif // TEST_THREAD

  // check samplerate
  if (com_data->samplerate != 0 && com_data->samplerate != samplerate){
	bool result = changeSamplerate(com_data->samplerate);
	if (result){
	  // new samplerate
	  samplerate = com_data->samplerate;
	  //	  cout << "samplerate: " << samplerate << endl << flush;
	}
	else {
	  // Yet: error
	}
  }

  return PROCESS_SUCCEEDED;
}

// transmit local buffer to global buffer
TRANSMIT_RESULT SoundThread::transmitBuffer()
{
  // received data size
  long receivedDataSize = com_data->data_size;

  // check
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
	if (outputLog){
	  cout << "[" << name << "] receivedDataSize = " << receivedDataSize << endl << flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive data for sound
  receivedDataSize = receiveData(buffer, receivedDataSize);
  // size check
  if (receivedDataSize <= 0){
	// error
	return TRANSMIT_DATASIZE_ERROR;
  }

#if TEST_THREAD
  printTimeInfo("got data");
#endif // TEST_THREAD

  // SOUND_TYPE_PCM
  // buffer[]         : PCM Data
  // receivedDataSize : Size of PCM Data
  // SOUND_TYPE_CELT
  // buffer[]         : CELT Data
  // receivedDataSize : Size of CELT Data

#if QTB_CELT_SUPPORT
  if (converter != 0){
	// for TEST : save CELT Data (append mode)
	if (settings->getOutputSoundDataToFile()){
	  outputReceivedData(receivedDataSize, "pcm/sound_output.raw");
	}
	// convert to PCM
	receivedDataSize = converter->convertToPCM(buffer, receivedDataSize);
	if (receivedDataSize == 0){
	  // Failed to convert to pcm
	  // Yet: error
	  return TRANSMIT_SUCCEEDED;
	}
#if TEST_THREAD
	printTimeInfo("decoded CELT");
#endif // TEST_THREAD
  }
#endif // QTB_CELT_SUPPORT

  // buffer[]         : PCM Data
  // receivedDataSize : Size of PCM Data

  // for TEST : save PCM Data (append)
  if (settings->getOutputSoundDataToFile()){
	outputReceivedData(receivedDataSize, "pcm/" QTB_SOUND_OUTPUT_FILENAME);
  }

  // put PCM data into sound buffer
  if (settings->getOnSound()){
	// put into soundBuffer
	int putSize = soundBuffer->put(buffer, receivedDataSize);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into sound buffer
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}
  }

  // put PCM data into sound device
  if (settings->getOnSound()){
	TRANSMIT_RESULT result = putPCMDataIntoSoundDevice();
	if (result != TRANSMIT_SUCCEEDED){
	  return result;
	}
  }

#if TEST_THREAD
	printTimeInfo("transfered PCM");
#endif // TEST_THREAD

  return TRANSMIT_SUCCEEDED;
}

// connected
void SoundThread::connectedToServer()
{
  // reset samplerate
  samplerate = 0;

  NetThread::connectedToServer();
}

// shutdown connection
#if defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)
void SoundThread::shutdownConnection()
{
  if (sock_sound != INVALID_SOCKET){
	shutdown(sock_sound, SD_BOTH);
	closesocket(sock_sound);
	sock_sound = INVALID_SOCKET;
  }

  // stop audiouOutput
  if (audioOutput != 0){
	audioOutput->stop();
  }

  NetThread::shutdownConnection();
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// receive data
long SoundThread::receiveData(char *buf, long size)
{
  return NetThread::receiveData(sock_sound, buf, size);
}

// put PCM data into sound device
TRANSMIT_RESULT SoundThread::putPCMDataIntoSoundDevice()
{
  // for cache
  static long soundCacheSize = 0;
  static long soundCacheSizeForLog = 0;
  static int soundCacheTime = -1;

  // check audio output
  if (audioOutput == 0){
	// NOT supported sample rate
	return TRANSMIT_SUCCEEDED;
  }

  // check sound cache time
  if (soundCacheTime != settings->getSoundCacheTime()){
	// update cacheSize
	soundCacheTime = settings->getSoundCacheTime();
	soundCacheSize = (long)(samplerate * 2 * 2 * (qreal)soundCacheTime/1000);
	soundCacheSizeForLog = soundCacheSize;
	if (settings->getOutputLog()){
	  cout << "soundCacheSize = " << soundCacheSize << endl << flush;
	}
  }

  if (settings->getOutputLog()){
	double cacheRate = 0;
	if (soundCacheSizeForLog != 0)
	  cacheRate = (double)(soundBuffer->getSize())/soundCacheSizeForLog * 100.0;
	cout << "[SoundThread] Sound Cache Rate : " << cacheRate << endl << flush;
  }

  // write into sound buffer
  if (soundBuffer->getSize() > soundCacheSize){
	if (audioOutput->state() != QAudio::StoppedState){
	  //	  soundCacheSize = 0;

	  int chunks = audioOutput->bytesFree()/(audioOutput->periodSize());

	  while(chunks){
		qint64 len = soundBuffer->getSize(audioOutput->periodSize());

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
	else {
	  // start output
	  output = audioOutput->start();
	}
  }

  return TRANSMIT_SUCCEEDED;
}

// change samplerate
bool SoundThread::changeSamplerate(SAMPLERATE samplerate)
{
  if (settings->getOutputLog()){
	cout << "[SoundThread] changeSamplerate(" << samplerate << ")" << endl << flush;
  }

  // audio format
  QAudioFormat format;

  // setting for sound format
  format.setSampleRate((int)samplerate);
  format.setChannelCount(2); // channel
  format.setSampleSize(16); // bits
  format.setCodec("audio/pcm"); // PCM
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  // audio device information
  const QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
  // supported Sample Rates
  if (settings->getOutputLog()){
	QList<int> sampleRatesList = deviceInfo.supportedSampleRates();
	cout << "supported Sample Rates : ";
	for(QList<int>::iterator i = sampleRatesList.begin(); i != sampleRatesList.end(); i++){
	  cout << (int)(*i) << " ";
	}
	cout << endl << flush;
  }
  if (!deviceInfo.isFormatSupported(format)){
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

  // clean sound buffer
  soundBuffer->clear();

  // create Audio Output
  if (audioOutput != 0){
	audioOutput->stop();
	delete audioOutput;
	audioOutput = 0;
  }
  audioOutput = new QAudioOutput(deviceInfo, format);

  // stateChanged
#if defined(DEBUG)
  connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(handleStateChanged(QAudio::State)));
#endif // defined(DEBUG)

#if QTB_CELT_SUPPORT
  // setup converter
  if (converter != 0){
	delete converter;
	converter = 0;
  }
  if (com_data->sound_type == SOUND_TYPE_CELT){
	converter = new Converter_CELT(samplerate, 2);
  }
#endif // QTB_CELT_SUPPORT

  // delete files
  if (settings->getOutputSoundDataToFile()){
	QFile pcmFile("pcm/" QTB_SOUND_OUTPUT_FILENAME);
	pcmFile.remove();
  }

  return true;
}

// output received data
void SoundThread::outputReceivedData(long receivedDataSize, const char *filename)
{
  fstream file;
  file.open(filename, ios::out | ios::binary | ios::app);
  if (file.is_open()){
	file.write(buffer, receivedDataSize);
	file.close();
  }
}

// create .wav file
void SoundThread::createWavFile(int pcmFileSize)
{
  // check
  if (pcmFileSize <= 0)
	return;

  fstream file;
  char filename[] = "pcm/" QTB_SOUND_OUTPUT_WAV_FILENAME;
  file.open(filename, ios::out | ios::binary);
  if (file.is_open()){
	// 1) Riff Header
	RiffHeader riffHeader;
#if 1 // for Coverity Scan
	memcpy(&riffHeader.riff, RIFF_ID, sizeof(riffHeader.riff));
#else
	strncpy((char *)&riffHeader.riff, RIFF_ID, sizeof(riffHeader.riff));
#endif
	riffHeader.size = pcmFileSize + sizeof(RiffHeader) + sizeof(FormatChunk) + sizeof(DataChunk) - 8;
#if 1 // for Coverity Scan
	memcpy(&riffHeader.type, RIFF_TYPE, sizeof(riffHeader.type));
#else
	strncpy((char *)&riffHeader.type, RIFF_TYPE, sizeof(riffHeader.type));
#endif
	file.write((char *)&riffHeader, sizeof(riffHeader));

	// 2) FormatChunk
	FormatChunk formatChunk;
#if 1 // for Coverity Scan
	memcpy(&formatChunk.id, FORMAT_CHUNK_ID, sizeof(formatChunk.id));
#else
	strncpy((char *)&formatChunk.id, FORMAT_CHUNK_ID, sizeof(formatChunk.id));
#endif
	formatChunk.size = FORMAT_CHUNK_SIZE;
	formatChunk.format = WAVE_FORMAT_PCM;
	formatChunk.channels = 2;
	formatChunk.samplerate = samplerate;
	formatChunk.bytepersec = 16/8 * formatChunk.channels * formatChunk.samplerate;
	formatChunk.blockalign = 16/8 * formatChunk.channels;
	formatChunk.bitswidth = 16; // 16 bits (2 bytes)
	file.write((char *)&formatChunk, sizeof(formatChunk));

	// 3) DataChunk
	DataChunk dataChunk;
#if 1 // for Coverity Scan
	memcpy(&dataChunk.id, DATA_CHUNK_ID, sizeof(dataChunk.id));
#else
	strncpy((char *)&dataChunk.id, DATA_CHUNK_ID, sizeof(dataChunk.id));
#endif
	dataChunk.size = pcmFileSize;
	file.write((char *)&dataChunk, sizeof(dataChunk));

	// copy PCM raw data to DataChunk(waveformData[])
	char in_filename[] = "pcm/" QTB_SOUND_OUTPUT_FILENAME;
	fstream in_file;
	in_file.open(in_filename, ios::in | ios::binary);
	if (in_file.is_open()){
	  while(pcmFileSize > 0){
		char buf[512*1024]; // 512KB buffer
		in_file.read(buf, 512*1024);
		int size = in_file.gcount();
		if (size > 0){
		  file.write(buf, size);
		  pcmFileSize -= size;
		}
	  }
	  in_file.close();
	}
	file.close();
  }
}

#if defined(DEBUG)
// audio state change event
void SoundThread::handleStateChanged(QAudio::State state)
{
  cout << "state = " << state << endl << flush;
}
#endif // defined(DEBUG)

} // end of namespace qtbrynhildr
