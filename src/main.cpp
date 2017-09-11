// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QApplication>
#if QTB_PUBLIC_MODE6_SUPPORT
#include <QClipboard>
#endif // QTB_PUBLIC_MODE6_SUPPORT
#if !QTB_BRYNHILDR2_SUPPORT
#include <QCursor>
#endif // !QTB_BRYNHILDR2_SUPPORT
#include <QLocale>
#include <QThread>
#include <QTranslator>
#if QTB_SPLASH_SCREEN
#include <QSplashScreen>
#endif /* QTB_SPLASH_SCREEN */

// Local Header
#include "config.h"
#include "option.h"
#include "qtbrynhildr.h"
#include "version.h"

// for DEBUG
#include "debug.h"

//using namespace qtbrynhildr;

int main(int argc, char *argv[])
{
  // for TEST and DEBUG
  if (!qtbrynhildr::checkProtocolHeader(false)){
	cout << "Failed: checkProtocolHeader(). header size or member offset error!" << endl << flush;
	::exit(EXIT_FAILURE);
  }

#if defined(Q_OS_WIN)
  // disabled IME
  ImmDisableIME(0);
#endif // defined(Q_OS_WIN)

  QApplication app(argc, argv);

  // analyze options
  qtbrynhildr::Option *option = new qtbrynhildr::Option(argc, argv);

  // set window icon
  QIcon windowIcon(QPixmap(QTB_ICON_FILENAME));
  app.setWindowIcon(windowIcon);

  // for Translation
  QTranslator appTranslator;
  if (qtbrynhildr::QTB_TRANSLATION && !option->getNoTransFlag()){
	bool result =
	  appTranslator.load(QTB_TRANSLATION_FILE_PATH
						 QTB_TRANSLATION_FILE_PREFIX
						 + QLocale::system().name(),
						 //					   qApp->applicationDirPath());
						 app.applicationDirPath());
	if (result){
	  //	  cout << "Found translation file." << endl << flush;
	  app.installTranslator(&appTranslator);
	}
	else {
	  cout << "Not found translation file. qtbrynhildr_*.qm" << endl << flush;
	}
  }
  // for Translation Qt
  QTranslator appTranslatorQt;
  if (!option->getNoTransFlag()){
	bool result =
	  appTranslatorQt.load("translations/"
						   "qt_"
						   + QLocale::system().name(),
						   //					   qApp->applicationDirPath());
						   app.applicationDirPath());
	if (result){
	  //	  cout << "Found translation file." << endl << flush;
	  app.installTranslator(&appTranslatorQt);
	}
	else {
	  cout << "Not found translation file. qt_*.qm" << endl << flush;
	}
  }

  // construct QtBrynhildr
#if QTB_PUBLIC_MODE6_SUPPORT
  // clipboard
  QClipboard *clipboard = QApplication::clipboard();
  qtbrynhildr::QtBrynhildr *qtbrynhildr = new qtbrynhildr::QtBrynhildr(option, clipboard);
#else // QTB_PUBLIC_MODE6_SUPPORT
  qtbrynhildr::QtBrynhildr *qtbrynhildr = new qtbrynhildr::QtBrynhildr(option);
#endif // QTB_PUBLIC_MODE6_SUPPORT
  if (qtbrynhildr->getShutdownFlag()){
	// shutdown now
	return EXIT_SUCCESS;
  }

  // display splash screen
#if QTB_SPLASH_SCREEN
  QSplashScreen splash;
  splash.setPixmap(QPixmap(QTB_SPLASH_SCREEN_FILENAME));
  splash.show();

  Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

  splash.showMessage(QObject::tr("Setting up the main window ..."),
					  topRight, Qt::white);

  // wait for 1 second
  QThread::sleep(1);
#endif /* QTB_SPLASH_SCREEN */

#if defined(Q_OS_WIN)
  // install native event filter
  app.installNativeEventFilter(qtbrynhildr->getMainWindow());
#endif // defined(Q_OS_WIN)

  // display main window
  qtbrynhildr->setFocus(Qt::ActiveWindowFocusReason);
  qtbrynhildr->show();

#if QTB_SPLASH_SCREEN
  // undisplay splash screen
  splash.finish(&qtbrynhildr);
#endif /* QTB_SPLASH_SCREEN */

  // event loop
  int result;
  result = app.exec();

  // restore cursor
  app.restoreOverrideCursor();

  // delete QtBrynhildr
  delete qtbrynhildr;

  // delete option
  delete option;

  return result;
}
