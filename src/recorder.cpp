// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// recorder.cpp

// Common Header
#include "common.h"

#if QTB_RECORDER

// System Header
#include <cstring>
#include <iostream>

// Qt Header

// Local Header
#include "parameters.h"
#include "recorder.h"
#include "version.h"

// temporary filename
#define RECORDER_TMP_FILENAME "qtbrynhildr.tmp"

namespace qtbrynhildr {

// constructor
Recorder::Recorder(Settings *settings)
  :
  settings(settings),
  header(new FileHeader),
  counter(0),
  dataSize(0),
  checkSum(0),
  filename(RECORDER_TMP_FILENAME),
  // for DEBUG
  outputLog(false)
{
  // initialization
  bodyEntry.counter = 0;
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

// start recording
void Recorder::startRecording()
{
  file.open(filename, ios::out | ios::binary);
  if (file.is_open()){
	dataSize = checkSum = 0;
	settings->setOnRecordingControl(true);
	if (outputLog)
	  cout << "open : " << filename  << endl << flush;
  }
}

// stop recording
void Recorder::stopRecording(const char* saveFileName)
{
  if (file.is_open()){
	file.close();
	if (outputLog)
	  cout << "close : " << endl << flush;
  }
  settings->setOnRecordingControl(false);

  // make fileName (header + raw)
#if 1 // copy for TEST
  fstream in_file;
  fstream out_file;
  in_file.open(filename, ios::in | ios::binary);
  out_file.open(saveFileName, ios::out | ios::binary);
  // write FileHeader
  makeFileHeader();
  out_file.write((char*)header, sizeof(FileHeader));
  while(true){
	char buf[sizeof(BodyEntry)];
	in_file.read(buf, sizeof(BodyEntry));
	if (in_file.eof())
	  break;
	out_file.write(buf, sizeof(BodyEntry));
  }
  in_file.close();
  out_file.close();
  remove(filename);
#else
  if (rename(filename, saveFileName) == 0){
	if (outputLog)
	  cout << "succeeded to rename()." << endl << flush;
  }
  else {
	cout << "failed to rename()." << endl << flush;
  }
#endif
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
	  if (file.is_open()){
		file.write((char*)&bodyEntry, sizeof(BodyEntry));
		dataSize += sizeof(BodyEntry);
		checkSum += sizeof(BodyEntry); // for TEST
		counter++;
		if (outputLog)
		  cout << "Write : " << bodyEntry.counter << " : com_data" << endl << flush;
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
  char loadFileName[QTB_MAXPATHLEN+1];
  snprintf(loadFileName, QTB_MAXPATHLEN, "%s", settings->getReplayingControlFileName());
  file.open(loadFileName, ios::in | ios::binary);
  if (file.is_open()){
	// read FileHeader and check
	file.read((char*)header, sizeof(FileHeader));
	if (!checkFileHeader()){
	  // illegal header
	  file.close();
	  return;
	}
	counter = 0;
	dataSize = checkSum = 0;
	settings->setOnReplayingControl(true);
	bodyEntry.counter = 0;
	if (outputLog)
	  cout << "succeded top open : " << loadFileName  << endl << flush;
  }
  else {
	if (outputLog)
	  cout << " failed to open : " << loadFileName  << endl << flush;
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
	// read next bodyEntry
	if (file.is_open()){
	  file.read((char*)&bodyEntry, sizeof(BodyEntry));
	  dataSize += sizeof(BodyEntry);
	  checkSum += sizeof(BodyEntry); // for TEST
	  counter++;
	  if (outputLog)
		cout << "read next entry! : " << counter << endl << flush;
	  if (file.eof()){
		stopReplaying();
	  }
	}
  }

  // (2)
  bodyEntry.counter -= 1;
  return &bodyEntry.com_data;
}

// make file header
void Recorder::makeFileHeader()
{
  // magic
  memcpy(header->magic, QTB_RECORDER_MAGIC, QTB_RECORDER_MAGIC_LENGTH);

  // time
  header->time = time(&header->time);

  // file length
  header->length = dataSize + sizeof(FileHeader);

  // server name
  strncpy(header->server, qPrintable(settings->getServerName()), 63);
  header->server[63] = '\0';

  // type of server OS
  header->serverType = settings->getServerType();

  // desktop width
  header->width = settings->getDesktopWidth();

  // desktop height
  header->height = settings->getDesktopHeight();

  // checksum
  header->checksum = checkSum;

  // version of Qt Brynhildr
  header->version = QTB_VERSION_NUMBER;
}

// check file header
bool Recorder::checkFileHeader()
{
  // magic
  if (memcmp(header->magic, QTB_RECORDER_MAGIC, QTB_RECORDER_MAGIC_LENGTH) != 0){
	cout << "illegal magic!" << endl << flush;
	return false;
  }

  // server name
  if (strncmp(header->server, qPrintable(settings->getServerName()), 63) != 0){
	// different server name
	cout << "illegal server!" << endl << flush;
	return false;
  }

  // type of server OS
  if (header->serverType != settings->getServerType()){
	cout << "illegal server type!" << endl << flush;
	return false;
  }

  return true;
}

} // end of namespace qtbrynhildr

#endif // QTB_RECORDER
