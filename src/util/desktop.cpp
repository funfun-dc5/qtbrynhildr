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
#include "desktop.h"

namespace qtbrynhildr {

#if QT_VERSION < 0x060000

// constructor
Desktop::Desktop()
  :maxImageDataSize(0)
  // for DEBUG
  ,outputLog(false)
{
  desktop = QApplication::desktop();

  if (outputLog){
	std::cout << "[Desktop] desktop total width  = " << desktop->width() << std::endl;
	std::cout << "[Desktop] desktop total height = " << desktop->height() << std::endl;
  }

  screens = QGuiApplication::screens();

  if (outputLog){
	int screenCount = screens.count();
	for (int i = 0; i < screenCount; i++){
	  std::cout << "[Desktop] screen : " << i << std::endl;
	  QScreen *screen = screens.at(i);
	  std::cout << "[Desktop] width  = " << screen->geometry().width() << std::endl;
	  std::cout << "[Desktop] height = " << screen->geometry().height() << std::endl;
	}
  }

  currentScreenNumber = desktop->screenNumber();
  currentScreen = screens.at(currentScreenNumber)->geometry();
  if (outputLog){
	std::cout << "[Desktop] current screen : " << currentScreenNumber << std::endl;
	std::cout << "[Desktop] width  = " << currentScreen.width() << std::endl;
	std::cout << "[Desktop] height = " << currentScreen.height() << std::endl;
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
	std::fstream file;
	char sizeStr[100];
	file.open(LINUX_SHMMAX, std::ios::in);
	if (file.is_open()){
	  file.getline(sizeStr, 100);
	  maxImageDataSize = strtoul(sizeStr, 0, 10);
	  //	  std::cout << "sizeStr = " << sizeStr << std::endl << std::flush;
	  //	  std::cout << "maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
	  file.close();
	}
	else {
	  std::cout << "[Desktop] Failed to open " LINUX_SHMMAX << std::endl << std::flush;
	}
  }
  //  std::cout << "[Desktop] maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
#endif

  // log std::flush
  if (outputLog){
	std::cout << std::flush;
  }
}

// destructor
Desktop::~Desktop()
{
}

#else // QT_VERSION >= 0x060000

// constructor
Desktop::Desktop()
  :maxImageDataSize(0)
  // for DEBUG
  ,outputLog(false)
{
  currentScreen = QApplication::primaryScreen();

  if (outputLog){
	std::cout << "[Desktop] desktop total width  = " << currentScreen->geometry().width() << std::endl;
	std::cout << "[Desktop] desktop total height = " << currentScreen->geometry().height() << std::endl;
  }

  screens = QApplication::screens();

  if (outputLog){
	int screenCount = screens.count();
	for (int i = 0; i < screenCount; i++){
	  std::cout << "[Desktop] screen : " << i << std::endl;
	  QScreen *screen = screens.at(i);
	  std::cout << "[Desktop] width  = " << screen->geometry().width() << std::endl;
	  std::cout << "[Desktop] height = " << screen->geometry().height() << std::endl;
	}
  }

#if 0 // for TEST
  qDebug() << "name = " << currentScreen->name();
  qDebug() << "model = " << currentScreen->model();
  qDebug() << "geometry = " << currentScreen->geometry();
  qDebug() << "availableGeometry = " << currentScreen->availableGeometry();
  qDebug() << "virtualGeometry = " << currentScreen->virtualGeometry();
  qDebug() << "logicalDotsPerInchX = " << currentScreen->logicalDotsPerInchX();
  qDebug() << "logicalDotsPerInchY = " << currentScreen->logicalDotsPerInchY();
  qDebug() << "physicalDotsPerInchX = " << currentScreen->physicalDotsPerInchX();
  qDebug() << "physicalDotsPerInchY = " << currentScreen->physicalDotsPerInchY();
#endif // 0 // for TEST

#if defined(Q_OS_LINUX)
#define LINUX_SHMMAX "/proc/sys/kernel/shmmax"
  // check max image size (for x11)
  if (QSysInfo::kernelType() == "linux"){
	std::fstream file;
	char sizeStr[100];
	file.open(LINUX_SHMMAX, std::ios::in);
	if (file.is_open()){
	  file.getline(sizeStr, 100);
	  maxImageDataSize = strtoul(sizeStr, 0, 10);
	  //	  std::cout << "sizeStr = " << sizeStr << std::endl << std::flush;
	  //	  std::cout << "maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
	  file.close();
	}
	else {
	  std::cout << "[Desktop] Failed to open " LINUX_SHMMAX << std::endl << std::flush;
	}
  }
  //  std::cout << "[Desktop] maxImageDataSize = " << maxImageDataSize << std::endl << std::flush;
#endif

  // log flush
  if (outputLog){
	std::cout << std::flush;
  }
}

// destructor
Desktop::~Desktop()
{
}

#endif // QT_VERSION >= 0x060000

} // end of namespace qtbrynhildr
