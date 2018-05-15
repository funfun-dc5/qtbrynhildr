// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstdlib>
#include <fstream>

// Qt Header
#include <QApplication>

// Local Header
#include "util/desktop.h"

namespace qtbrynhildr {

// constructor
Desktop::Desktop()
  :maxImageDataSize(0)
  // for DEBUG
  ,outputLog(false)
{
  desktop = QApplication::desktop();

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

  currentScreenNumber = desktop->screenNumber();
  currentScreen = desktop->screenGeometry(currentScreenNumber);
  if (outputLog){
	cout << "[Desktop] current screen : " << currentScreenNumber << endl;
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
  if (kernelVersion.startsWith("10.")){			// Windows 10
#if !QTB_NEW_DESKTOPWINDOW
	correctWindowWidth = 2;
	correctWindowHeight = 3;
#endif // !QTB_NEW_DESKTOPWINDOW
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = 0;
  }
  else if (kernelVersion.startsWith("6.3")){	// Windows 8.1
#if !QTB_NEW_DESKTOPWINDOW
	correctWindowWidth = 2;
	correctWindowHeight = 3;
#endif // !QTB_NEW_DESKTOPWINDOW
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
  else if (kernelVersion.startsWith("6.2")){	// Windows 8
#if !QTB_NEW_DESKTOPWINDOW
	correctWindowWidth = 2;
	correctWindowHeight = 3;
#endif // !QTB_NEW_DESKTOPWINDOW
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
  else if (kernelVersion.startsWith("6.1")){	// Windows 7
#if !QTB_NEW_DESKTOPWINDOW
	correctWindowWidth = 2;
	correctWindowHeight = 4;
#endif // !QTB_NEW_DESKTOPWINDOW
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -4;
  }
  else {
	// NOT supported Version
#if !QTB_NEW_DESKTOPWINDOW
	correctWindowWidth = 2;
	correctWindowHeight = 3;
#endif // !QTB_NEW_DESKTOPWINDOW
	heightOfMenuBarInHiding = -1;
	heightOfStatusBarInHiding = -2;
  }
#elif defined(Q_OS_LINUX)
#if defined(Q_OS_ANDROID)
  // Android
#if !QTB_NEW_DESKTOPWINDOW
  correctWindowWidth = 2;
  correctWindowHeight = 4;
#endif // !QTB_NEW_DESKTOPWINDOW
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#else // defined(Q_OS_ANDROID)
  // Linux Desktop
#if !QTB_NEW_DESKTOPWINDOW
  correctWindowWidth = 2;
  correctWindowHeight = 2;
#endif // !QTB_NEW_DESKTOPWINDOW
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#endif // defined(Q_OS_ANDROID)
#elif defined(Q_OS_CYGWIN)
  // Cygwin
#if !QTB_NEW_DESKTOPWINDOW
  correctWindowWidth = 2;
  correctWindowHeight = 2;
#endif // !QTB_NEW_DESKTOPWINDOW
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#elif defined(Q_OS_FREEBSD)
  // FreeBSD
#if !QTB_NEW_DESKTOPWINDOW
  correctWindowWidth = 0;
  correctWindowHeight = 0;
#endif // !QTB_NEW_DESKTOPWINDOW
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#elif defined(Q_OS_OSX)
  // Darwin
#if !QTB_NEW_DESKTOPWINDOW
  correctWindowWidth = 0;
  correctWindowHeight = 0;
#endif // !QTB_NEW_DESKTOPWINDOW
  heightOfMenuBarInHiding = -1;
  heightOfStatusBarInHiding = 0;
#endif

  // environment variables
  char *envval = 0;
  envval = getenv("TESTMODE");
  if (envval != 0){
	// display current parameters
	cout << "== DESKTOP TESTMODE ==" << endl;
	cout << "correctWindowWidth        : " << correctWindowWidth << endl;
	cout << "correctWindowHeight       : " << correctWindowHeight << endl;
	cout << "heightOfMenuBarInHiding   : " << heightOfMenuBarInHiding << endl;
	cout << "heightOfStatusBarInHiding : " << heightOfStatusBarInHiding << endl << endl << flush;
  }

  // set parameters
  envval = getenv("CORRECT_WINDOW_WIDTH");
  if (envval != 0){
	cout << "correctWindowWidth (" << correctWindowWidth << ") -> ";
	correctWindowWidth = strtol(envval, 0, 10);
	cout << correctWindowWidth << endl << flush;
  }
  envval = getenv("CORRECT_WINDOW_HEIGHT");
  if (envval != 0){
	cout << "correctWindowHeight (" << correctWindowHeight << ") -> ";
	correctWindowHeight = strtol(envval, 0, 10);
	cout << correctWindowHeight << endl << flush;
  }
  envval = getenv("HEIGHT_OF_MENU_BAR_IN_HIDING");
  if (envval != 0){
	cout << "heightOfMenuBarInHiding (" << heightOfMenuBarInHiding << ") -> ";
	heightOfMenuBarInHiding = strtol(envval, 0, 10);
	cout << heightOfMenuBarInHiding << endl << flush;
  }
  envval = getenv("HEIGHT_OF_STATUS_BAR_IN_HIDING");
  if (envval != 0){
	cout << "heightOfStatusBarInHiding (" << heightOfStatusBarInHiding << ") -> ";
	heightOfStatusBarInHiding = strtol(envval, 0, 10);
	cout << heightOfStatusBarInHiding << endl << flush;
  }

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
