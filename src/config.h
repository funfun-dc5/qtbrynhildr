// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// config.h

#ifndef CONFIG_H
#define CONFIG_H

namespace qtbrynhildr {

// os type
#if defined(Q_OS_WIN)
// windows type
#define QTB_OS_WIN	1
#elif defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX)
// unix type
#define QTB_OS_UNIX	1
#endif // defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX)

// for device type
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define QTB_TBALE 1
#else
#define QTB_DESKTOP 1
#endif

// main window size fixed mode
const bool QTB_FIXED_MAINWINDOW_SIZE	= true;

// desktop image scaling
const bool QTB_DESKTOP_IMAGE_SCALING	= true;

// cut desktop blank area
const bool QTB_CUT_DESKTOP_BLANK_AREA	= true;

// desktop full screen mode
#if defined(Q_OS_MAC) || defined(QTB_TABLET)
const bool QTB_DESKTOP_FULL_SCREEN		= false;
#else // defined(Q_OS_MAC) || defined(QTB_TABLET)
const bool QTB_DESKTOP_FULL_SCREEN		= true;
#endif // defined(Q_OS_MAC) || defined(QTB_TABLET)

// stays on top
const bool QTB_DESKTOP_STAYS_ON_TOP		= true;

// desktop frame control
const bool QTB_DESKTOP_FRAME_CONTROL	= true;

// software keyboard
const bool QTB_SOFTWARE_KEYBOARD_AND_BUTTON		= true;

// translation
const bool QTB_TRANSLATION				= true;

// in testing
const bool QTB_IN_TESTING				= false;

// recorder
#define QTB_RECORDER				1

// splash screen
#define QTB_SPLASH_SCREEN			0

// cryptogram
#define QTB_CRYPTOGRAM				0

// debug
#define QTB_DEBUG					1
#define QTB_DEBUG_KEYBOARD			false
#define QTB_DEBUG_MOUSE				false

} // end of namespace qtbrynhildr

#endif // CONFIG_H
