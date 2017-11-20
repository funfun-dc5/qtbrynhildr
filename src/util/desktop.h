// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

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

#if !QTB_NEW_DESKTOPWINDOW
  // get correct widow width
  int getCorrectWindowWidth() const
  {
	return correctWindowWidth;
  }

  // get correct widow height
  int getCorrectWindowHeight() const
  {
	return correctWindowHeight;
  }
#endif // !QTB_NEW_DESKTOPWINDOW

  // get height of menu bar in hiding
  int getHeightOfMenuBarInHiding() const
  {
	return heightOfMenuBarInHiding;
  }

  // get height of status bar in hiding
  int getHeightOfStatusBarInHiding() const
  {
	return heightOfStatusBarInHiding;
  }

private:
  // desktop informaation
  QDesktopWidget *desktop;

  // current screen number
  int currentScreenNumber;

  // current screen
  QRect currentScreen;

  // max image size
  unsigned long maxImageDataSize;

#if !QTB_NEW_DESKTOPWINDOW
  // correct window width
  int correctWindowWidth;

  // correct window height
  int correctWindowHeight;
#endif // !QTB_NEW_DESKTOPWINDOW

  // height of menu bar in hiding
  int heightOfMenuBarInHiding;

  // height of status bar in hiding
  int heightOfStatusBarInHiding;

  // output log flag
  const bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // DESKTOP_H
