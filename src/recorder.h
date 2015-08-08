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
#define QTB_RECORDER_MAGIC 			"QTBF"
#define QTB_RECORDER_MAGIC_LENGTH	4

namespace qtbrynhildr {

// Recorder
class Recorder
{
public:
  // constructor
  Recorder(Settings *settings);
  // destructor
  ~Recorder();

  // put com_data
  void putCOM_DATA(COM_DATA *com_data);

  // get com_data
  COM_DATA *getCOM_DATA();

private:
  // make file header
  void makeFileHeader();

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

	// version of server OS

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

  // file stream
  fstream file;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // RECORDER_H
