// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

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
#else // !(defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_IOS) || defined(Q_OS_CYGWIN))
#error "Not support Q_OS for QTB_NET_*"
#endif // !(defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_IOS) || defined(Q_OS_CYGWIN))

// for WinSock1 (TEST)
#define QTB_NET_WINSOCK1 0

// for device type
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define QTB_DEV_TOUCHPANEL
#elif defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_CYGWIN)
#define QTB_DEV_DESKTOP
#else // !(defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_IOS) || defined(Q_OS_CYGWIN))
#error "Not support Q_OS for QTB_DEV_*"
#endif // !(defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OSX) || defined(Q_OS_IOS) || defined(Q_OS_CYGWIN))

// main window size fixed mode
const bool QTB_FIXED_MAINWINDOW_SIZE	= true;

// desktop image scaling
#if defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_SCALING	= true;
#else // !defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_SCALING	= false;
#endif // !defined(QTB_DEV_DESKTOP)

// desktop image capture
#if defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_CAPTURE	= true;
#else // !defined(QTB_DEV_DESKTOP)
const bool QTB_DESKTOP_IMAGE_CAPTURE	= false;
#endif // !defined(QTB_DEV_DESKTOP)

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
#else // !defined(QTB_DEV_DESKTOP)
const bool QTB_SCROLL_MODE				= false;
#endif // !defined(QTB_DEV_DESKTOP)

// viewer mode
const bool QTB_VIEWER_MODE				= true;

// log view dialog
const bool QTB_LOG_VIEW					= true;

// translation
const bool QTB_TRANSLATION				= true;

// in testing
const bool QTB_IN_TESTING				= false;

#if 0 // into qtbrynhildr.pro
// recorder
#if defined(QTB_DEV_DESKTOP)
#define QTB_RECORDER				1
#endif // defined(QTB_DEV_DESKTOP)
#endif // into qtbrynhildr.pro

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
#else // !defined(QTB_DEV_DESKTOP)
#define QTB_DRAG_AND_DROP_SUPPORT	0
#endif // !defined(QTB_DEV_DESKTOP)

// desktop compress mode
#if defined(QTB_DEV_DESKTOP)
#define QTB_DESKTOP_COMPRESS_MODE	1
#else // !defined(QTB_DEV_DESKTOP)
#define QTB_DESKTOP_COMPRESS_MODE	0
#endif // !defined(QTB_DEV_DESKTOP)

// disable plugins support
#define QTB_PLUGINS_DISABLE_SUPPORT	1

#if 0 // into qtbrynhildr.pro
// auto complete
#define QTB_AUTO_COMPLETE			1
#endif // into qtbrynhildr.pro

// manual page
#define QTB_MANUAL_PAGE				1

// update check (need SSL)
#if 0 // into qtbrynhildr.pro
#if defined(QTB_DEV_DESKTOP)
#define QTB_UPDATECHECK				1
#else // !defined(QTB_DEV_DESKTOP)
#define QTB_UPDATECHECK				0
#endif // !defined(QTB_DEV_DESKTOP)
#endif // 0 // into qtbrynhildr.pro

// help browser (need SSL)
#if QTB_HELP_BROWSER && !QTB_UPDATECHECK
#undef QTB_HELP_BROWSER
#define QTB_HELP_BROWSER 0
#endif // QTB_HELP_BROWSER && !QTB_UPDATECHECK

// preference dialog
//#define QTB_PREFERENCE				1

// SIMD instruction support
//#define QTB_SIMD_SUPPORT			1

// sound push mode
#define QTB_SOUND_PUSH_MODE			1

// gray scale mode
#define QTB_GRAY_SCALE_MODE			1

// gray scale mode2
#define QTB_GRAY_SCALE_MODE2		0

#if QTB_GRAY_SCALE_MODE2
#undef QTB_GRAY_SCALE_MODE
#define QTB_GRAY_SCALE_MODE			1
#endif // QTB_GRAY_SCALE_MODE2

// LOAD BITMAP
#define QTB_LOAD_BITMAP				0

// ToolBar
#if defined(QTB_DEV_DESKTOP)
#define QTB_TOOLBAR					1
#else // !defined(QTB_DEV_DESKTOP)
#define QTB_TOOLBAR					0
#endif // !defined(QTB_DEV_DESKTOP)

// mouse tracking focus mode
#if defined(QTB_DEV_DESKTOP)
#define QTB_MOUSE_TRACKING_FOCUS_MODE	1
#else // !defined(QTB_DEV_DESKTOP)
#define QTB_MOUSE_TRACKING_FOCUS_MODE	0
#endif // !defined(QTB_DEV_DESKTOP)

// benchmark
#define QTB_BENCHMARK				1

// debug
#define QTB_DEBUG					0

  // test
#define QTB_TEST					1
} // end of namespace qtbrynhildr

#endif // CONFIG_H
