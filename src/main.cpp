// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// main.cpp

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
#include "qtbrynhildr.h"
#include "version.h"

// for DEBUG
#include "debug.h"

#if 1 // for TEST
// for CELT
#include "celt.h"
#endif // for TEST

//using namespace qtbrynhildr;

int main(int argc, char *argv[])
{
#if 1 // for TEST
  int error;
  CELTMode *mode = celt_mode_create(44100, 256, &error);
  cout << "celt_mode_create : error = " << error << endl << flush;
  celt_int32 skip = 0;
  if (mode != 0){
	celt_mode_info(mode, CELT_GET_LOOKAHEAD, &skip);
	cout << "skip = " << skip << endl << flush;
	celt_mode_destroy(mode);
  }
  else {
	cout << "Ouch: mode = 0" << endl << flush;
  }
#endif // for TEST
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

  // set window icon
  QIcon windowIcon(QPixmap(QTB_ICON_FILENAME));
  app.setWindowIcon(windowIcon);

  // set cursor
#if !QTB_BRYNHILDR2_SUPPORT
  app.setOverrideCursor(Qt::CrossCursor);
#else // !QTB_BRYNHILDR2_SUPPORT
  app.setOverrideCursor(Qt::ArrowCursor);
#endif // !QTB_BRYNHILDR2_SUPPORT

  // for Translation
  QTranslator appTranslator;
  if (qtbrynhildr::QTB_TRANSLATION){
	appTranslator.load(QTB_TRANSLATION_FILE_PATH
					   QTB_TRANSLATION_FILE_PREFIX
					   + QLocale::system().name(),
					   qApp->applicationDirPath());
	app.installTranslator(&appTranslator);
  }

  // construct QtBrynhildr
#if QTB_PUBLIC_MODE6_SUPPORT
  // clipboard
  QClipboard *clipboard = QApplication::clipboard();
  qtbrynhildr::QtBrynhildr qtbrynhildr(argc, argv, clipboard);
#else // QTB_PUBLIC_MODE6_SUPPORT
  qtbrynhildr::QtBrynhildr qtbrynhildr(argc, argv);
#endif // QTB_PUBLIC_MODE6_SUPPORT
  if (qtbrynhildr.getShutdownFlag()){
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
  app.installNativeEventFilter(qtbrynhildr.getMainWindow());
#endif // defined(Q_OS_WIN)

  // display main window
  qtbrynhildr.setFocus(Qt::ActiveWindowFocusReason);
  qtbrynhildr.show();

#if QTB_SPLASH_SCREEN
  // undisplay splash screen
  splash.finish(&qtbrynhildr);
#endif /* QTB_SPLASH_SCREEN */

  // event loop
  int result;
  result = app.exec();

  // restore cursor
  app.restoreOverrideCursor();

  return result;
}
