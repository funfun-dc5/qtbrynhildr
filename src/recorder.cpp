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

namespace qtbrynhildr {

// constructor
Recorder::Recorder(Settings *settings)
  :
  settings(settings),
  header(0),
  // for DEBUG
  outputLog(false)
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
	  static bool firstOpen = true;
	  // write to file
	  if (outputLog)
		cout << "Write : " << bodyEntry.counter << " : com_data" << endl << flush;
#if 1 // for TEST
	  char filename[QTB_MAXPATHLEN+1];
	  snprintf(filename, QTB_MAXPATHLEN, "%s.qtbf", "test");
	  if (!file.is_open()){
		if (firstOpen){
		  firstOpen = false;
		  file.open(filename , ios::out | ios::binary);
		}
		else {
		  file.open(filename, ios::out | ios::binary | ios::app);
		}
		if (file.is_open()){
		  file.write((char*)&bodyEntry, sizeof(BodyEntry));
		  file.close();
		}
	  }
	  else {
		// already open file
	  }
#endif // for TEST
	}
	// set new com_data
	bodyEntry.counter = 1;
	memcpy(&bodyEntry.com_data, com_data, sizeof(COM_DATA));
  }
}

// get com_data
COM_DATA *Recorder::getCOM_DATA()
{
  static bool done = false;

  // (1) open file sream
  if (!file.is_open() && !done){
	char filename[QTB_MAXPATHLEN+1];
	snprintf(filename, QTB_MAXPATHLEN, "%s.qtbf", "test");
	file.open(filename, ios::in | ios::binary);
	if (file.is_open()){
	  bodyEntry.counter = 0;
	  if (outputLog)
		cout << "open!" << endl << flush;
	}
	else {
	  return 0;
	}
  }
  else {
	if (!done){
	  if (file.eof()){
		done = true;
		file.close();
		if (outputLog)
		  cout << "close!" << endl << flush;
		return 0;
	  }
	}
	else {
	  return 0;
	}
  }

  // (2)
  if (bodyEntry.counter == 0){
	static int counter = 1;
	// read next bodyEntry
	file.read((char*)&bodyEntry, sizeof(BodyEntry));
	if (outputLog)
	  cout << "read next entry! : " << counter << endl << flush;
	counter++;
  }

  // (3)
  bodyEntry.counter -= 1;
  return &bodyEntry.com_data;
}

} // end of namespace qtbrynhildr
