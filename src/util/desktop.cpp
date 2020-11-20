// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstdlib>
#include <fstream>

// Qt Header
#include <QApplication>

#if defined(QTB_DEBUG)
#include <QDebug>
#endif // defined(QTB_DEBUG)

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

  screens = QGuiApplication::screens();

  if (outputLog){
	int screenCount = screens.count();
	for (int i = 0; i < screenCount; i++){
	  cout << "[Desktop] screen : " << i << endl;
	  QScreen *screen = screens.at(i);
	  cout << "[Desktop] width  = " << screen->geometry().width() << endl;
	  cout << "[Desktop] height = " << screen->geometry().height() << endl;
	}
  }

  currentScreenNumber = desktop->screenNumber();
  currentScreen = screens.at(currentScreenNumber)->geometry();
  if (outputLog){
	cout << "[Desktop] current screen : " << currentScreenNumber << endl;
	cout << "[Desktop] width  = " << currentScreen.width() << endl;
	cout << "[Desktop] height = " << currentScreen.height() << endl;
  }

#if 0 // for TEST
  qDebug() << "name = " << screens.at(currentScreenNumber)->name();
  qDebug() << "model = " << screens.at(currentScreenNumber)->model();
  qDebug() << "geometry = " << screens.at(currentScreenNumber)->geometry();
  qDebug() << "availableGeometry = " << screens.at(currentScreenNumber)->availableGeometry();
  qDebug() << "virtualGeometry = " << screens.at(currentScreenNumber)->virtualGeometry();
  qDebug() << "logicalDotsPerInchX = " << screens.at(currentScreenNumber)->logicalDotsPerInchX();
  qDebug() << "logicalDotsPerInchY = " << screens.at(currentScreenNumber)->logicalDotsPerInchY();
  qDebug() << "physicalDotsPerInchX = " << screens.at(currentScreenNumber)->physicalDotsPerInchX();
  qDebug() << "physicalDotsPerInchY = " << screens.at(currentScreenNumber)->physicalDotsPerInchY();
#endif // 0 // for TEST

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
