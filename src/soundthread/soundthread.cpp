// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#include <fstream>
#include <iostream>

// Qt Header
#include <QAudio>
#if QT_VERSION < 0x060000
#include <QAudioDeviceInfo>
#else // QT_VERSION >= 0x060000
#include <QMediaDevices>
#include <QAudioSink>
#endif // QT_VERSION >= 0x060000

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
  //outputLog = true; // for DEBUG

  // change thread sleep time
  //threadSleepTime = 10;

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

#if QTB_CELT_SUPPORT
  // converter
  if (converter != 0){
	delete converter;
	converter = 0;
  }
#endif // QTB_CELT_SUPPORT

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

  if (counter_graphics < NEXT_CONNECT_COUNT){
	return CONNECT_WAITED_COUNT;
  }

  // connect
  sock_sound = socketToServer();
  if (sock_sound == INVALID_SOCKET){
	return CONNECT_FAILED;
  }
  if (sock_sound == TIMEOUT_SOCKET){
	return CONNECT_FAILED_TIMEOUT;
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
	std::cout << "[" << name << "]" << " received data size (" << dataSize << ") != sizeof(COM_DATA)" << std::endl << std::flush; // error
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
	  //	  std::cout << "samplerate: " << samplerate << std::endl << std::flush;
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
  long dataSize = com_data->data_size;

  // check
  // size check
  if (dataSize < 0){
	// error
	return TRANSMIT_DATASIZE_ERROR;
  }
  if (dataSize == 0){
	// Nothing to do
	return TRANSMIT_SUCCEEDED;
  }
  if (dataSize > QTB_SOUND_LOCAL_BUFFER_SIZE){
	if (outputLog){
	  std::cout << "[" << name << "] dataSize = " << dataSize << std::endl << std::flush; // error
	}
	return TRANSMIT_DATASIZE_ERROR;
  }

  // receive data for sound
  long receivedDataSize = receiveData(buffer, dataSize);
  // size check
  if (receivedDataSize != dataSize){
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

  // no sound output check
  if (!hasSoundData(buffer, receivedDataSize)){
	return TRANSMIT_SUCCEEDED;
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

  //std::cout << "[" << name << "]  receivedDataSize = " << receivedDataSize << std::endl << std::flush; // error

#if QTB_SOUND_PUSH_MODE
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

#endif // QTB_SOUND_PUSH_MODE

  return TRANSMIT_SUCCEEDED;
}

// connected
void SoundThread::connectedToServer()
{
  // reset samplerate
  samplerate = 0;

  // succeeded to connect
  settings->setConnected(true);
  // connected
  emit connected();
  // refresh window
  emit refreshWindow();

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
	  std::cout << "soundCacheSize = " << soundCacheSize << std::endl << std::flush;
	}
  }

  if (settings->getOutputLog()){
	double cacheRate = 0;
	if (soundCacheSizeForLog != 0)
	  cacheRate = (double)(soundBuffer->getSize())/soundCacheSizeForLog * 100.0;
	std::cout << "[SoundThread] Sound Cache Rate : " << cacheRate << std::endl << std::flush;
  }

  // write into sound buffer
  if (soundBuffer->getSize() > soundCacheSize){
	if (audioOutput->state() != QAudio::StoppedState){
	  //	  soundCacheSize = 0;

#if 0 // for TEST
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
#else // 1 // for TEST
	  // get free size of buffer
	  int len = audioOutput->bytesFree();
	  // write PCM data
	  if (len != 0){
		qint64 result = output->write(soundBuffer->get(len), len);
		if (result != len){
		  // Failed to write
		  return TRANSMIT_FAILED_TRANSMIT_DEVICE_BUFFER;
		}
	  }
#endif // 1 // for TEST
	}
#if 0 // for TEST
	else {
	  // start output
	  output = audioOutput->start();
	}
#endif // 0 // for TEST
  }

  return TRANSMIT_SUCCEEDED;
}

// change samplerate
#if QT_VERSION < 0x060000
bool SoundThread::changeSamplerate(SAMPLERATE samplerate)
{
  if (settings->getOutputLog()){
	std::cout << "[SoundThread] changeSamplerate(" << samplerate << ")" << std::endl << std::flush;
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
  {
	QString str;
	str = "SoundType : " + QString::number(settings->getSoundType());
	emit outputLogMessage(PHASE_SOUND, str);
	str = "SoundCacheTime : " + QString::number(settings->getSoundCacheTime()) + " (ms)";
	emit outputLogMessage(PHASE_SOUND, str);
	str = "SampleRate : " + QString::number(samplerate) + " (Hz)";
	emit outputLogMessage(PHASE_SOUND, str);
  }
  // supported Sample Rates
  if (settings->getOutputLog()){
	QList<int> sampleRatesList = deviceInfo.supportedSampleRates();
	std::cout << "supported Sample Rates : ";
	for(QList<int>::iterator i = sampleRatesList.begin(); i != sampleRatesList.end(); i++){
	  std::cout << (int)(*i) << " ";
	}
	std::cout << std::endl << std::flush;
  }
  if (!settings->getOnSound()){
	// Sound OFF
	return true;
  }
  if (!deviceInfo.isFormatSupported(format)){
	if (audioOutput != 0){
	  audioOutput->stop();
	  delete audioOutput;
	  audioOutput = 0;
	}
	// NOT supported sample rate!
	if (!settings->getOnSoundCriticalMessageDisable()){
	  emit outputLogMessage(QTB_MSG_NOTSUPPORT_SAMPLE_RATE);
	}
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
  //  std::cout << "bufferSize: " << audioOutput->bufferSize() << std::endl << std::flush;
  //  std::cout << "periodSize: " << audioOutput->periodSize() << std::endl << std::flush;
  // set sound buffer size
  audioOutput->setBufferSize(QTB_SOUND_BUFFER_SIZE);

  // stateChanged
  connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(handleStateChanged(QAudio::State)));

#if defined(Q_OS_LINUX)
  // for Runtime Message of QAudio::State (Qt Static Library Version)
  // QObject::connect: Cannot queue arguments of type 'QAudio::State'
  // (Make sure 'QAudio::State' is registered using qRegisterMetaType().)
  qRegisterMetaType<QAudio::State>("QAudio::State");
#endif // defined(Q_OS_LINUX)

#if 0 // for TEST
  audioOutput->setNotifyInterval(1000); // 1000 (ms)
  connect(audioOutput, SIGNAL(notify()), SLOT(notify()));
#endif // 0 // for TEST

  // start output
  output = audioOutput->start();

  //  std::cout << "bufferSize: " << audioOutput->bufferSize() << std::endl << std::flush;
  //  std::cout << "periodSize: " << audioOutput->periodSize() << std::endl << std::flush;
  if (settings->getOutputLog()){
	QString str;
	str = "Sound Device BufferSize : " + QString::number(audioOutput->bufferSize()) + " (bytes)";
	emit outputLogMessage(PHASE_SOUND, str);
	str = "Sound Device PeriodSize : " + QString::number(audioOutput->periodSize()) + " (bytes)";
	emit outputLogMessage(PHASE_SOUND, str);
  }

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
#else // QT_VERSION >= 0x060000
bool SoundThread::changeSamplerate(SAMPLERATE samplerate)
{
  if (settings->getOutputLog()){
	std::cout << "[SoundThread] changeSamplerate(" << samplerate << ")" << std::endl << std::flush;
  }

  // audio device information
  const QAudioDevice defaultAudioOutput = QMediaDevices::defaultAudioOutput();

  // audio format
  QAudioFormat format = defaultAudioOutput.preferredFormat();

  // setting for sound format
  format.setSampleRate((int)samplerate);
  format.setChannelCount(2); // channel
  format.setSampleFormat(QAudioFormat::Int16); // 16bits
#if 0 // for TEST
  format.setCodec("audio/pcm"); // PCM
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);
#endif // 0 // for TEST

  {
	QString str;
	str = "SoundType : " + QString::number(settings->getSoundType());
	emit outputLogMessage(PHASE_SOUND, str);
	str = "SoundCacheTime : " + QString::number(settings->getSoundCacheTime()) + " (ms)";
	emit outputLogMessage(PHASE_SOUND, str);
	str = "SampleRate : " + QString::number(samplerate) + " (Hz)";
	emit outputLogMessage(PHASE_SOUND, str);
  }
  // supported Sample Rates
  if (settings->getOutputLog()){
	QString str = "Supported Sampling Rate (Hz): ";
	if (defaultAudioOutput.isNull()){
	  str =  str + " None";
	}
	else {
	  str = str +
		QString::number(defaultAudioOutput.minimumSampleRate()) + " -  " +
		QString::number(defaultAudioOutput.maximumSampleRate());
	}
	std::cout << qPrintable(str) << std::endl << std::flush;
  }
  if (!settings->getOnSound()){
	// Sound OFF
	return true;
  }
  if (!defaultAudioOutput.isFormatSupported(format)){
	if (audioOutput != 0){
	  audioOutput->stop();
	  delete audioOutput;
	  audioOutput = 0;
	}
	// NOT supported sample rate!
	if (!settings->getOnSoundCriticalMessageDisable()){
	  emit outputLogMessage(QTB_MSG_NOTSUPPORT_SAMPLE_RATE);
	}
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
  audioOutput = new QAudioSink(defaultAudioOutput, format);
  audioOutput->setBufferSize(QTB_SOUND_BUFFER_SIZE);

  // stateChanged
  connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(handleStateChanged(QAudio::State)));

#if defined(Q_OS_LINUX)
  // for Runtime Message of QAudio::State (Qt Static Library Version)
  // QObject::connect: Cannot queue arguments of type 'QAudio::State'
  // (Make sure 'QAudio::State' is registered using qRegisterMetaType().)
  qRegisterMetaType<QAudio::State>("QAudio::State");
#endif // defined(Q_OS_LINUX)

#if 0 // for TEST
  audioOutput->setNotifyInterval(1000); // 1000 (ms)
  connect(audioOutput, SIGNAL(notify()), SLOT(notify()));
#endif // 0 // for TEST

  // start output
  output = audioOutput->start();

  //  std::cout << "bufferSize: " << audioOutput->bufferSize() << std::endl << std::flush;
  //  std::cout << "periodSize: " << audioOutput->periodSize() << std::endl << std::flush;
  if (settings->getOutputLog()){
	QString str;
	str = "Sound Device BufferSize : " + QString::number(audioOutput->bufferSize()) + " (bytes)";
	emit outputLogMessage(PHASE_SOUND, str);
#if 0 // for TEST
	str = "Sound Device PeriodSize : " + QString::number(audioOutput->periodSize()) + " (bytes)";
	emit outputLogMessage(PHASE_SOUND, str);
#endif // 0 // for TEST
  }

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
#endif // QT_VERSION >= 0x060000

// output received data
void SoundThread::outputReceivedData(long receivedDataSize, const char *filename)
{
  std::fstream file;
  file.open(filename, std::ios::out | std::ios::binary | std::ios::app);
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

  std::fstream file;
  char filename[] = "pcm/" QTB_SOUND_OUTPUT_WAV_FILENAME;
  file.open(filename, std::ios::out | std::ios::binary);
  if (file.is_open()){
	// 1) Riff Header
	RiffHeader riffHeader;
#if 1 // for Coverity Scan
	memcpy(&riffHeader.riff, RIFF_ID, sizeof(riffHeader.riff));
#else // 0 // for Coverity Scan
	strncpy((char *)&riffHeader.riff, RIFF_ID, sizeof(riffHeader.riff));
#endif // 0 // for Coverity Scan
	riffHeader.size = pcmFileSize + sizeof(RiffHeader) + sizeof(FormatChunk) + sizeof(DataChunk) - 8;
#if 1 // for Coverity Scan
	memcpy(&riffHeader.type, RIFF_TYPE, sizeof(riffHeader.type));
#else // 0 // for Coverity Scan
	strncpy((char *)&riffHeader.type, RIFF_TYPE, sizeof(riffHeader.type));
#endif // 0 // for Coverity Scan
	file.write((char *)&riffHeader, sizeof(riffHeader));

	// 2) FormatChunk
	FormatChunk formatChunk;
#if 1 // for Coverity Scan
	memcpy(&formatChunk.id, FORMAT_CHUNK_ID, sizeof(formatChunk.id));
#else // 0 // for Coverity Scan
	strncpy((char *)&formatChunk.id, FORMAT_CHUNK_ID, sizeof(formatChunk.id));
#endif // 0 // for Coverity Scan
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
#else // 0 // for Coverity Scan
	strncpy((char *)&dataChunk.id, DATA_CHUNK_ID, sizeof(dataChunk.id));
#endif // 0 // for Coverity Scan
	dataChunk.size = pcmFileSize;
	file.write((char *)&dataChunk, sizeof(dataChunk));

	// copy PCM raw data to DataChunk(waveformData[])
	char in_filename[] = "pcm/" QTB_SOUND_OUTPUT_FILENAME;
	std::fstream in_file;
	in_file.open(in_filename, std::ios::in | std::ios::binary);
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


// audio state change event
void SoundThread::handleStateChanged(QAudio::State state)
{
#if defined(DEBUG)
  std::cout << "state = " << state << std::endl << std::flush;
#else // !defined(DEBUG)
  Q_UNUSED(state);
#endif // !defined(DEBUG)
}

#if 0 // for TEST
void SoundThread::notify()
{
  std::cout << "called notify" << std::endl << std::flush;
}
#endif // 0 // for TEST

} // end of namespace qtbrynhildr
