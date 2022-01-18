// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

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
  // desktop image
  static QImage *image;

protected:
  // data buffer
  char *buffer;

  // data size
  int size;

protected:
#if QTB_GRAY_SCALE_MODE
  // gray scale
  static bool onGrayScale;
#endif // QTB_GRAY_SCALE_MODE

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  Decoder();
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

#if QTB_GRAY_SCALE_MODE
  // set gray scale flag
  virtual void setOnGrayScale(bool onGrayScale) = 0;
#endif // QTB_GRAY_SCALE_MODE

  // output data to file
  bool outputDataToFile(char *buf, int len, int frameId);

protected:

private:
  
};

} // end of namespace qtbrynhildr

#endif // DECODER_H
