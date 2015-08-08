// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// recorder.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt Header

// Local Header
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

// record com_data
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
	  cout << "Write Data : counter = " << bodyEntry.counter << " : com_data" << endl << flush;
	}
	// set new com_data
	bodyEntry.counter = 1;
	memcpy(&bodyEntry.com_data, com_data, sizeof(COM_DATA));
  }
}

} // end of namespace qtbrynhildr
