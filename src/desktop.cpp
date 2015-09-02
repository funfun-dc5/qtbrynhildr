// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// desktop.cpp

// Common Header
#include "common.h"

// System Header
#include <cstdlib>
#include <fstream>

// Qt Header
#include <QApplication>

// Local Header
#include "desktop.h"

namespace qtbrynhildr {

// constructor
Desktop::Desktop()
  :
  maxImageDataSize(0),
  // for DEBUG
  outputLog(false)
{
  QDesktopWidget *desktop = QApplication::desktop();

  if (outputLog){
	cout << "[Desktop] desktop total width  = " << desktop->width() << endl;
	cout << "[Desktop] desktop total height = " << desktop->height() << endl;
  }

  if (outputLog){
	int screenCount = desktop->screenCount();
	for (int i = 0; i < screenCount; i++){
	  cout << "[Desktop] screen : " << i << endl;
	  QWidget *screen = desktop->screen(i);
	  cout << "[Desktop] width  = " << screen->width() << endl;
	  cout << "[Desktop] height = " << screen->height() << endl;
	}
  }

  int screenNumber = desktop->screenNumber();
  currentScreen = desktop->screenGeometry(screenNumber);
  if (outputLog){
	cout << "[Desktop] current screen : " << screenNumber << endl;
	cout << "[Desktop] width  = " << currentScreen.width() << endl;
	cout << "[Desktop] height = " << currentScreen.height() << endl;
  }

#if defined(Q_OS_LINUX)
#define LINUX_SHMMAX "/proc/sys/kernel/shmmax"
  // check max image size (for x11)
  if (QSysInfo::kernelType() == "linux"){
	fstream file;
	char sizeStr[100];
	file.open(LINUX_SHMMAX, ios::in);
	if (file.is_open()){
	  file.getline(sizeStr, 100);
	  maxImageDataSize = strtoul(sizeStr, 0, 10);
	  //	  cout << "sizeStr = " << sizeStr << endl << flush;
	  //	  cout << "maxImageDataSize = " << maxImageDataSize << endl << flush;
	  file.close();
	}
	else {
	  cout << "[Desktop] Failed to open " LINUX_SHMMAX << endl << flush;
	}
  }
  //  cout << "[Desktop] maxImageDataSize = " << maxImageDataSize << endl << flush;
#endif

  // set correct window width and height
#if defined(Q_OS_WIN)
  // OS Version
  QString kernelVersion = QSysInfo::kernelVersion();
  if (kernelVersion.startsWith("6.3")){			// Windows 8.1
	correctWindowWidth = 2;
	correctWindowHeight = 3;
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
  else if (kernelVersion.startsWith("6.2")){	// Windows 8
	correctWindowWidth = 2;
	correctWindowHeight = 3;
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
  else if (kernelVersion.startsWith("6.1")){	// Windows 7
	correctWindowWidth = 2;
	correctWindowHeight = 4;
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -4;
  }
  else {
	// NOT supported Version
	correctWindowWidth = 2;
	correctWindowHeight = 3;
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
#elif defined(Q_OS_LINUX)
  // Linux
  correctWindowWidth = 2;
  correctWindowHeight = 2;
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#elif defined(Q_OS_MAC)
  // Darwin
  correctWindowWidth = 2;
  correctWindowHeight = 2;
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#endif

  // log flush
  if (outputLog){
	cout << flush;
  }
}

// destructor
Desktop::~Desktop()
{
}

} // end of namespace qtbrynhildr
