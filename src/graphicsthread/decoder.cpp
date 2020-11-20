// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <fstream>

// Qt Header

// Local Header
#include "decoder.h"
#include "parameters.h"
#include "settings.h"

namespace qtbrynhildr {

// desktop image
QImage*  Decoder::image = new QImage;

#if QTB_GRAY_SCALE_MODE
// gray scale flag
bool Decoder::onGrayScale = false;
#endif // QTB_GRAY_SCALE_MODE

// constructor
Decoder::Decoder()
  :buffer(0)
  ,size(0)
  // for DEBUG
  ,outputLog(false)
{
}

// destructor
Decoder::~Decoder()
{
}

// pre-process
bool Decoder::preprocess(char *buffer, int size)
{
  if (outputLog){
	static bool flag = true;
	if (flag){
	  cout << "Decoder::preprocess()" << endl << flush;
	  flag = false;
	}
  }

  // set parameters
  this->buffer = buffer;
  this->size = size;

  return true;
}

// output data to file
bool Decoder::outputDataToFile(char *buf, int len, int frameId)
{
  fstream file;
  char filename[QTB_MAXPATHLEN+1];
  int result;

  result = snprintf(filename, QTB_MAXPATHLEN, "jpg/%s_%06d.%s",
					QTB_GRAPHICS_OUTPUT_FILENAME_PREFIX,
					frameId,
					suffix());
  if (result > 0 && result <= QTB_MAXPATHLEN){
	file.open(filename, ios::out | ios::binary | ios::trunc);
	if (file.is_open()){
	  file.write(buf, len);
	  file.close();
	}
  }
  else {
	// snprintf() error
	cout << "[Decoder] snprintf() error!" << endl << flush;
	return false;
  }

  return true;
}

} // end of namespace qtbrynhildr
