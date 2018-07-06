// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOP_H
#define DESKTOP_H
// Common Header
#include "common/common.h"

// Qt Header
#include <QDesktopWidget>
#include <QRect>
#include <QSize>

namespace qtbrynhildr {

// Desktop
class Desktop
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // desktop informaation
  QDesktopWidget *desktop;

  // current screen number
  int currentScreenNumber;

  // current screen
  QRect currentScreen;

  // max image size
  unsigned long maxImageDataSize;

  // output log flag
  const bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  Desktop();
  // destructor
  ~Desktop();

  // get current screen
  QRect getCurrentScreen()
  {
	if (currentScreenNumber != desktop->screenNumber()){
	  currentScreen = desktop->screenGeometry(currentScreenNumber);
	}
	return currentScreen;
  }

  // check current screen is changed
  bool isChangedCurrentScreen()
  {
	return currentScreenNumber != desktop->screenNumber();
  }

  // get max image size
  unsigned long getMaxImageDataSize() const
  {
	return maxImageDataSize;
  }
};

} // end of namespace qtbrynhildr

#endif // DESKTOP_H
