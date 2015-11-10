// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// config.h

#ifndef CONFIG_H
#define CONFIG_H

namespace qtbrynhildr {

// network type
#if defined(Q_OS_WIN)
// windows socket (WinSock)
#define QTB_NET_WIN	1
#elif defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_IOS) || defined(Q_OS_CYGWIN)
// unix socket
#define QTB_NET_UNIX	1
#else
#error "Not support Q_OS for QTB_NET_*"
#endif

// for device type
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define QTB_DEV_TABLET 1
#elif defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_CYGWIN)
#define QTB_DEV_DESKTOP 1
#else
#error "Not support Q_OS for QTB_DEV_*"
#endif

// main window size fixed mode
const bool QTB_FIXED_MAINWINDOW_SIZE	= true;

// desktop image scaling
const bool QTB_DESKTOP_IMAGE_SCALING	= true;

// cut desktop blank area
const bool QTB_CUT_DESKTOP_BLANK_AREA	= true;

// desktop full screen mode
#if defined(Q_OS_OSX) || defined(QTB_DEV_TABLET)
const bool QTB_DESKTOP_FULL_SCREEN		= false;
#else // defined(Q_OS_OSX) || defined(QTB_DEV_TABLET)
const bool QTB_DESKTOP_FULL_SCREEN		= true;
#endif // defined(Q_OS_OSX) || defined(QTB_DEV_TABLET)

// stays on top
const bool QTB_DESKTOP_STAYS_ON_TOP		= true;

// desktop frame control
const bool QTB_DESKTOP_FRAME_CONTROL	= true;

// software keyboard and button
const bool QTB_SOFTWARE_KEYBOARD_AND_BUTTON		= true;

// scroll mode
#if defined(QTB_DEV_DESKTOP)
const bool QTB_SCROLL_MODE				= true;
#else // defined(QTB_DEV_DESKTOP)
const bool QTB_SCROLL_MODE				= false;
#endif // defined(QTB_DEV_DESKTOP)

// translation
const bool QTB_TRANSLATION				= true;

// in testing
const bool QTB_IN_TESTING				= false;

// recorder
#if defined(QTB_DEV_TABLET)
#define QTB_RECORDER				0
#else // defined(QTB_DEV_TABLET)
#define QTB_RECORDER				1
#endif // defined(QTB_DEV_TABLET)

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
