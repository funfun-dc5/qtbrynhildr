// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// recorder.h

#ifndef RECORDER_H
#define RECORDER_H
// System Header
#include <ctime>
#include <fstream>

// Local Header
#include "protocols.h"
#include "settings.h"

// magic
#define QTB_RECORDER_MAGIC 			"QTB"
#define QTB_RECORDER_MAGIC_LENGTH	3

namespace qtbrynhildr {

// Recorder
class Recorder
{
public:
  // constructor
  Recorder(Settings *settings);
  // destructor
  ~Recorder();

  // start recording
  void startRecording();

  // stop recording
  void stopRecording(const char *saveFileName);

  // start replaying
  void startReplaying();

  // stop replaying
  void stopReplaying();

  // put com_data
  void putCOM_DATA(COM_DATA *com_data);

  // get com_data
  COM_DATA *getCOM_DATA();

private:
  // make file header
  void makeFileHeader();

  // check file header
  bool checkFileHeader();

private:
  // file header
  typedef struct {
	// magic
	char magic[QTB_RECORDER_MAGIC_LENGTH];

	// time
	time_t time;

	// file length
	int length;

	// server name
	char server[64];

	// type of server OS
	SERVER_TYPE serverType;

	// desktop width
	int width;

	// desktop height
	int height;

	// checksum
	int checksum;

	// version of Qt Brynhildr
	int version;

  } FileHeader;

  // body entry
  typedef struct {
	int counter;
	COM_DATA com_data;
  } BodyEntry;

  // settings
  Settings *settings;

  // file header
  FileHeader *header;

  // body entry
  BodyEntry bodyEntry;

  // counter of body entry
  int counter;

  // data size
  int dataSize;

  // checksum
  int checkSum;

  // file stream
  fstream file;

  // temporary filename
  const char *filename;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // RECORDER_H
