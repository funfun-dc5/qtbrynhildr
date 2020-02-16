// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef DECODER_H
#define DECODER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QImage>

// Local Header


namespace qtbrynhildr {

class Decoder
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
protected:
  // data buffer
  char *buffer;

  // data size
  int size;

  // desktop image
  QImage *image;

protected:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  Decoder(QImage *image);
  // destructor
  virtual ~Decoder();

  // get decoder name
  virtual const char* name() = 0;

  // get suffix
  virtual const char* suffix() = 0;

  // pre-process
  virtual bool preprocess(char *buffer, int size);

  // get desktop image
  virtual QImage* getDesktopImage(int numOfThread) = 0;

  // output data to file
  bool outputDataToFile(char *buf, int len, int frameId);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_H
