// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

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

// for WinSock1 (TEST)
#define QTB_NET_WINSOCK1 0

// for device type
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define QTB_DEV_TOUCHPANEL
#elif defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_CYGWIN)
#define QTB_DEV_DESKTOP
#else
#error "Not support Q_OS for QTB_DEV_*"
#endif

// main window size fixed mode
const bool QTB_FIXED_MAINWINDOW_SIZE	= true;

// desktop image scaling
const bool QTB_DESKTOP_IMAGE_SCALING	= true;

// desktop image capture
#if defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_CAPTURE	= true;
#else // defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_CAPTURE	= false;
#endif // defined(QTB_DEV_DESKTOP)

// cut desktop blank area
const bool QTB_CUT_DESKTOP_BLANK_AREA	= true;

// desktop full screen mode
const bool QTB_DESKTOP_FULL_SCREEN		= true;

// stays on top
const bool QTB_DESKTOP_STAYS_ON_TOP		= true;

// desktop scale fixed
const bool QTB_DESKTOP_SCALE_FIXED		= true;

// window size fixed
const bool QTB_WINDOW_SIZE_FIXED		= true;

// desktop frame rate control
const bool QTB_DESKTOP_FRAMERATE_CONTROL= true;

// software keyboard and button
//#define QTB_SOFTWARE_KEYBOARD_AND_BUTTON	1

// scroll mode
#if defined(QTB_DEV_DESKTOP)
const bool QTB_SCROLL_MODE				= true;
#else // defined(QTB_DEV_DESKTOP)
const bool QTB_SCROLL_MODE				= false;
#endif // defined(QTB_DEV_DESKTOP)

// log view dialog
const bool QTB_LOG_VIEW					= true;

// translation
const bool QTB_TRANSLATION				= true;

// in testing
const bool QTB_IN_TESTING				= false;

// recorder
#if defined(QTB_DEV_DESKTOP)
#define QTB_RECORDER				1
#endif // defined(QTB_DEV_DESKTOP)

// splash screen
#define QTB_SPLASH_SCREEN			0

// cryptogram
#define QTB_CRYPTOGRAM				0

// celt support
#ifndef QTB_CELT_SUPPORT
#define QTB_CELT_SUPPORT			1
#endif

// extra mouse button support
#define QTB_EXTRA_BUTTON_SUPPORT	1

// drag and drop support
#if defined(QTB_DEV_DESKTOP)
#define QTB_DRAG_AND_DROP_SUPPORT	1
#else // defined(QTB_DEV_DESKTOP)
#define QTB_DRAG_AND_DROP_SUPPORT	0
#endif // defined(QTB_DEV_DESKTOP)
// public mode 7 support
#define QTB_PUBLIC_MODE7_SUPPORT	1
// desktop compress mode
#define QTB_DESKTOP_COMPRESS_MODE	1

// disable plugins support
#define QTB_PLUGINS_DISABLE_SUPPORT	1

// auto complete
#define QTB_AUTO_COMPLETE			1

// update check
#if defined(QTB_DEV_DESKTOP)
#define QTB_UPDATECHECK				1
#else // defined(QTB_DEV_DESKTOP)
#define QTB_UPDATECHECK				0
#endif // defined(QTB_DEV_DESKTOP)

// preference dialog
//#define QTB_PREFERENCE				1

// SIMD INSTRUCTIONS SUPPORT
#define QTB_SIMD_SUPPORT			1

// TEST CODE
#define QTB_TEST_CODE				0

// debug
#define QTB_DEBUG					1

} // end of namespace qtbrynhildr

#endif // CONFIG_H
