// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// recorder.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt Header

// Local Header
#include "parameters.h"
#include "recorder.h"

#if QTB_RECORDER

namespace qtbrynhildr {

// constructor
Recorder::Recorder(Settings *settings)
  :
  settings(settings),
  header(0),
  // for DEBUG
  outputLog(true)
{
}

// destructor
Recorder::~Recorder()
{
  // delete objects
  if (header != 0){
	delete header;
	header = 0;
  }
}

// make file header
void Recorder::makeFileHeader()
{
  // create header
  if (header == 0){
	header = new FileHeader();
  }

  // magic
  memcpy(header->magic, QTB_RECORDER_MAGIC, QTB_RECORDER_MAGIC_LENGTH);

  // time
  header->time = time(&header->time);

  // file length
  header->length = 0;

  // server name

  // version of server OS

  // desktop width
  //  header->width

  // desktop height
  //  header->height

  // checksum

  // version of Qt Brynhildr
  //  header->version
}

// start recording
void Recorder::startRecording()
{
  char filename[QTB_MAXPATHLEN+1];
  snprintf(filename, QTB_MAXPATHLEN, "%s", settings->getRecordingControlFileName());
  file.open(filename , ios::out | ios::binary);
  if (file.is_open()){
	settings->setOnRecordingControl(true);
	if (outputLog)
	  cout << "open : " << filename  << endl << flush;
  }
}

// stop recording
void Recorder::stopRecording()
{
  if (file.is_open()){
	file.close();
	if (outputLog)
	  cout << "close : " << endl << flush;
  }
  settings->setOnRecordingControl(false);
}

// put com_data
void Recorder::putCOM_DATA(COM_DATA *com_data)
{
  //  printHeader();
  if (memcmp(&bodyEntry.com_data, com_data, sizeof(COM_DATA)) == 0){
	// found same com_data
	bodyEntry.counter += 1;
  }
  else {
	// found new com_data
	if (bodyEntry.counter > 0){
	  // write to file
	  if (outputLog)
		cout << "Write : " << bodyEntry.counter << " : com_data" << endl << flush;
		if (file.is_open()){
		  file.write((char*)&bodyEntry, sizeof(BodyEntry));
		}
	}
	// set new com_data
	bodyEntry.counter = 1;
	memcpy(&bodyEntry.com_data, com_data, sizeof(COM_DATA));
  }
}

// start replaying
void Recorder::startReplaying()
{
  char filename[QTB_MAXPATHLEN+1];
  snprintf(filename, QTB_MAXPATHLEN, "%s", settings->getReplayingControlFileName());
  file.open(filename , ios::in | ios::binary);
  if (file.is_open()){
	settings->setOnReplayingControl(true);
	bodyEntry.counter = 0;
	if (outputLog)
	  cout << "open : " << filename  << endl << flush;
  }
}

// stop replaying
void Recorder::stopReplaying()
{
  if (file.is_open()){
	file.close();
	if (outputLog)
	  cout << "close : " << endl << flush;
  }
  settings->setOnReplayingControl(false);
}

// get com_data
COM_DATA *Recorder::getCOM_DATA()
{
  // (1)
  if (bodyEntry.counter == 0){
	static int counter = 1;
	// read next bodyEntry
	if (file.is_open()){
	  file.read((char*)&bodyEntry, sizeof(BodyEntry));
	  if (outputLog)
		cout << "read next entry! : " << counter << endl << flush;
	  counter++;
	  if (file.eof()){
		stopReplaying();
	  }
	}
  }

  // (2)
  bodyEntry.counter -= 1;
  return &bodyEntry.com_data;
}

} // end of namespace qtbrynhildr

#endif // QTB_RECORDER
