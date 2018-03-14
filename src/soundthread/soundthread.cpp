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

namespace qtbrynhildr {

// constructor
SoundThread::SoundThread(Settings *settings)
  :
  NetThread("SoundThread", settings),
  soundBuffer(0),
  soundBufferSize(0),
  soundCacheTime(-1),
  samplerate(0),
#if !QTB_NEWFEATURE_SB
  audioOutput(0),
  output(0),
#endif // !QTB_NEWFEATURE_SB
  samplerateChangeCounter(0),
#if QTB_CELT_SUPPORT
  converter(0),
#endif //QTB_CELT_SUPPORT
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

#if !QTB_NEWFEATURE_SB
  // audio output
  if (audioOutput != 0){
	audioOutput->stop();
#if !defined(QTB_RPI3) // for Segmentation Fault on rpi3
	delete audioOutput;
#endif // !defined(QTB_RPI3) // for Segmentation Fault on rpi3
	audioOutput = 0;
  }
#endif // !QTB_NEWFEATURE_SB

  // output wav file
  if (settings->getOutputSoundDataToFile() && settings->getOutputSoundDataToWavFile() &&
	  samplerateChangeCounter == 1){
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

  // reset state
  samplerateChangeCounter = 0;

  return CONNECT_SUCCEEDED;
}

// process for header
PROCESS_RESULT SoundThread::processForHeader()
{
  // receive header
  long dataSize;
  dataSize = receiveData(sock_sound, (char *)com_data, sizeof(COM_DATA));
  if (dataSize != sizeof(COM_DATA)){
	// error
#if 0 // for TEST
	cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << endl << flush; // error
#endif // for TEST
	return PROCESS_NETWORK_ERROR;
  }

  // print received header
  if (outputLog){
	printHeader();
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
	if (outputLog){
	  cout << "[SoundThread] receivedDataSize = " << receivedDataSize << endl << flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive PCM raw Data
  receivedDataSize = receiveData(sock_sound, buffer, receivedDataSize);
  // no data
  if (receivedDataSize <= 0){
	// Nothing to do
	return TRANSMIT_DATASIZE_ERROR;
  }

#if QTB_CELT_SUPPORT
  if (converter != 0){
	// for DEBUG : save CELT Data (append mode)
	if (settings->getOutputSoundDataToFile()){
	  fstream file;
	  char filename[] = "pcm/sound_output.raw";
	  file.open(filename, ios::out | ios::binary | ios::app);
	  if (file.is_open()){
		file.write(buffer, receivedDataSize);
		file.close();
	  }
	}
	// convert to PCM
	receivedDataSize = converter->convertToPCM(buffer, receivedDataSize);
	if (receivedDataSize == 0){
	  // Failed to convert to pcm
	  // Yet: error
	  return TRANSMIT_SUCCEEDED;
	}
  }
#endif // QTB_CELT_SUPPORT

  // buffer[]         : PCM Data
  // receivedDataSize : Size of PCM Data

  // for DEBUG : save PCM Data (append)
  if (settings->getOutputSoundDataToFile()){
	fstream file;
	char filename[] = "pcm/" QTB_SOUND_OUTPUT_FILENAME;
	file.open(filename, ios::out | ios::binary | ios::app);
	if (file.is_open()){
	  file.write(buffer, receivedDataSize);
	  file.close();
	}
  }

#if !QTB_NEWFEATURE_SB
  // put PCM data into sound buffer
  if (settings->getOnSound()){
	// for cache
	static long soundCacheSize = 0;
	static long soundCacheSizeForLog = 0;

	// check audio output
	if (audioOutput == 0){
	  // NOT supported sample rate
	  return TRANSMIT_SUCCEEDED;
	}
#endif // !QTB_NEWFEATURE_SB

	// put into soundBuffer
	int putSize = soundBuffer->put(buffer, receivedDataSize);
	if (putSize != receivedDataSize){
	  // error for put()
	  // Failed to put into sound buffer
	  return TRANSMIT_FAILED_PUT_BUFFER;
	}

	//cout << "sound buffer size : " << soundBuffer->getSize() << endl << flush; // for TEST

#if !QTB_NEWFEATURE_SB
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

#if 0 // for DEBUG
	static int count = 0;
	if (count % 500 == 0){
	  cout << "[SoundThread] receivedDataSize = " << receivedDataSize << endl;
	  cout << "[SoundThread] size = " << soundBuffer->getSize() << ", count = " << count << endl;
	  cout << "[SoundThread] bytesFree() =  " << audioOutput->bytesFree() << endl << flush;
	}
	count++;
#endif
  }
#endif // !QTB_NEWFEATURE_SB

  return TRANSMIT_SUCCEEDED;
}

// connected
void SoundThread::connectedToServer()
{
  // reset samplerate
  samplerate = 0;

  // delete files
  if (settings->getOutputSoundDataToFile()){
	QFile pcmFile("pcm/" QTB_SOUND_OUTPUT_FILENAME);
	pcmFile.remove();
  }

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

#if !QTB_NEWFEATURE_SB
  // stop audiouOutput
  if (audioOutput != 0){
	audioOutput->stop();
  }
#endif // !QTB_NEWFEATURE_SB

  NetThread::shutdownConnection();
}
#endif // defined(QTB_NET_WIN) || defined(QTB_NET_UNIX)

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// change samplerate
bool SoundThread::changeSamplerate(SAMPLERATE samplerate)
{
  if (settings->getOutputLog()){
	cout << "[SoundThread] changeSamplerate(" << samplerate << ")" << endl << flush;
  }

#if !QTB_NEWFEATURE_SB
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

#else // !QTB_NEWFEATURE_SB
  // clean sound buffer
  soundBuffer->clear();
#endif // !QTB_NEWFEATURE_SB

  // change counter up
  samplerateChangeCounter++;

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

  return true;
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

#if !QTB_NEWFEATURE_SB
#if defined(DEBUG)
// audio state change event
void SoundThread::handleStateChanged(QAudio::State state)
{
  cout << "state = " << state << endl << flush;
}
#endif // defined(DEBUG)
#endif // !QTB_NEWFEATURE_SB

} // end of namespace qtbrynhildr
