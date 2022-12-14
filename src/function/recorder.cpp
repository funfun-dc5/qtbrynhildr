// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

#if QTB_RECORDER

// System Header
#include <cstring>
#include <iostream>

// Qt Header
#include <QDir>

// Local Header
#include "parameters.h"
#include "recorder.h"
#include "version.h"

// temporary filename
#define RECORDER_TMP_FILENAME "qtbrynhildr.tmp"

namespace qtbrynhildr {

// constructor
Recorder::Recorder(Settings *settings)
  :settings(settings)
  ,header(new FileHeader)
  ,counter(0)
  ,dataSize(0)
  ,checkSum(0)
  // for DEBUG
  ,outputLog(false)
{
  // initialization
  bodyEntry.counter = 0;
  memset(&bodyEntry.com_data, 0, sizeof(COM_DATA)); // for warning

  // filename
  QString tempFileName = QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() +
												  RECORDER_TMP_FILENAME);
  filename = qPrintable(tempFileName);
  //  std::cout << "filename = " << filename << std::endl << std::flush;
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
  file.open(filename, std::ios::out | std::ios::binary);
  if (file.is_open()){
	dataSize = checkSum = 0;
	settings->setOnRecordingControl(true);
	if (outputLog)
	  std::cout << "open : " << filename  << std::endl << std::flush;
  }
}

// stop recording
void Recorder::stopRecording(const char* saveFileName)
{
  if (file.is_open()){
	file.close();
	if (outputLog)
	  std::cout << "close : " << std::endl << std::flush;
  }
  settings->setOnRecordingControl(false);

  // make fileName (header + raw)
#if 0 // copy for TEST
  std::fstream in_file;
  std::fstream out_file;
  in_file.open(filename, std::ios::in | std::ios::binary);
  out_file.open(saveFileName, std::ios::out | std::ios::binary);
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
#else // copy for TEST
  if (rename(filename, saveFileName) == 0){
	if (outputLog)
	  std::cout << "succeeded to rename()." << std::endl << std::flush;
  }
  else {
	std::cout << "failed to rename()." << std::endl << std::flush;
  }
#endif // copy for TEST
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
		  std::cout << "Write : " << bodyEntry.counter << " : com_data" << std::endl << std::flush;
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
  file.open(loadFileName, std::ios::in | std::ios::binary);
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
	  std::cout << "succeded top open : " << loadFileName  << std::endl << std::flush;
  }
  else {
	if (outputLog)
	  std::cout << " failed to open : " << loadFileName  << std::endl << std::flush;
  }
}

// stop replaying
void Recorder::stopReplaying()
{
  if (file.is_open()){
	file.close();
	if (outputLog)
	  std::cout << "close : " << std::endl << std::flush;
  }
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
		std::cout << "read next entry! : " << counter << std::endl << std::flush;
	  if (file.eof()){
		stopReplaying();
	  }
	}
	else {
	  // file is closed
	  settings->setOnReplayingControl(false);
	  return 0;
	}
  }
  else if (bodyEntry.counter < 0){
	settings->setOnReplayingControl(false);
	return 0;
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
	std::cout << "illegal magic!" << std::endl << std::flush;
	return false;
  }

  // server name
  if (strncmp(header->server, qPrintable(settings->getServerName()), 63) != 0){
	// different server name
	std::cout << "illegal server!" << std::endl << std::flush;
	return false;
  }

  // type of server OS
  if (header->serverType != settings->getServerType()){
	std::cout << "illegal server type!" << std::endl << std::flush;
	return false;
  }

  // fixed version id bug from Ver.2.1.0 to Ver.2.1.5-rc1
  if (header->version >= 210 && header->version <= 215){
	header->version = 2100 + header->version - 210;
  }

  return true;
}

} // end of namespace qtbrynhildr

#endif // QTB_RECORDER
