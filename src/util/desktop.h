// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef DESKTOP_H
#define DESKTOP_H
// Common Header
#include "common/common.h"

// Qt Header
#if QT_VERSION < 0x060000
#include <QDesktopWidget>
#endif // QT_VERSION < 0x060000
#include <QGuiApplication>
#include <QList>
#include <QRect>
#include <QSize>
#include <QScreen>

namespace qtbrynhildr {

#if QT_VERSION < 0x060000

// Desktop
class Desktop
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // desktop informaation
  QDesktopWidget *desktop;

  // screens
  QList<QScreen*> screens;

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
	  currentScreenNumber = desktop->screenNumber();
	  currentScreen = screens.at(currentScreenNumber)->geometry();
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

  // get logical dots per inch x
  int getLogicalDotsPerInchX() const
  {
	return screens.at(currentScreenNumber)->logicalDotsPerInchX();
  }

  // get logical dots per inch y
  int getLogicalDotsPerInchY() const
  {
	return screens.at(currentScreenNumber)->logicalDotsPerInchY();
  }
};

#else // QT_VERSION >= 0x060000

// Desktop
class Desktop
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // screens
  QList<QScreen*> screens;

  // current screen
  QScreen *currentScreen;

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
	return currentScreen->geometry();
  }

  // check current screen is changed
  bool isChangedCurrentScreen()
  {
	return true; // for TEST
  }

  // get max image size
  unsigned long getMaxImageDataSize() const
  {
	return maxImageDataSize;
  }

  // get logical dots per inch x
  int getLogicalDotsPerInchX() const
  {
	return currentScreen->logicalDotsPerInchX();
  }

  // get logical dots per inch y
  int getLogicalDotsPerInchY() const
  {
	return currentScreen->logicalDotsPerInchY();
  }
};

#endif // QT_VERSION >= 0x060000

} // end of namespace qtbrynhildr

#endif // DESKTOP_H
