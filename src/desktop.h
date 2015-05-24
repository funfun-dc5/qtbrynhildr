// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// desktop.h

#ifndef DESKTOP_H
#define DESKTOP_H
// Common Header
#include "common.h"

// Qt Header
#include <QDesktopWidget>
#include <QRect>

namespace qtbrynhildr {

// Desktop
class Desktop
{
public:
  // constructor
  Desktop();
  // destructor
  ~Desktop();

  // get current screen
  QRect getCurrentScreen() const
  {
	return currentScreen;
  }

  // get max image size
  unsigned long getMaxImageDataSize() const
  {
	return maxImageDataSize;
  }

private:
  // current screen
  QRect currentScreen;

  // max image size
  unsigned long maxImageDataSize;

  // output log flag
  const bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // DESKTOP_H
