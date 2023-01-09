// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef PROTOCOLS_x86_H
#define PROTOCOLS_x86_H
// Common Header
#include "common/common.h"

namespace qtbrynhildr {

// Protocol Header for PUBLIC MODE of Brynhildr
// for x86 architecture

// server version
typedef long	SERVER_VERSION;
#define SERVER_VERSION_BRYNHILDR	1	// brynhildr
#define SERVER_VERSION_BRYNHILDR2	2	// brynhildr 2

// data type
typedef char	DATA_TYPE;
#define DATA_TYPE_DATA		1	// data
#define DATA_TYPE_CLIPBOARD	2	// clipboard
#define DATA_TYPE_FILE		3	// file

// thread
typedef char	THREAD;
#define THREAD_CONTROL		1	// thread for control
#define THREAD_GRAPHICS		2	// thread for graphics
#define THREAD_SOUND		3	// thread for sound

// sound type
typedef	char	SOUND_TYPE;
#define SOUND_TYPE_OFF		0	// sound OFF
#define SOUND_TYPE_PCM		1	// sound PCM
#if QTB_CELT_SUPPORT
#define SOUND_TYPE_CELT		2	// sound CELT
#endif // QTB_CELT_SUPPORT

// encryption
typedef char	ENCRYPTION;
#define ENCRYPTION_OFF		0	// encryption OFF
#define ENCRYPTION_ON		1	// encryption ON

// check digit
typedef char	CHECK_DIGIT;
typedef long	CHECK_DIGIT_VALUE;

// sample rate
typedef long	SAMPLERATE;

// control
typedef char	CONTROL;
#define CONTROL_OFF			0	// control off
#define CONTROL_ON			1	// control on

// plugins disable
#if QTB_PLUGINS_DISABLE_SUPPORT
#define PLUGINS_DISABLE_OFF	0 // plugins disable off
#define PLUGINS_DISABLE_ON	1 // plugins disable on
#endif // QTB_PLUGINS_DISABLE_SUPPORT

// file drop
#define FILEDROP_OFF	0 // filedrop off
#define FILEDROP_ON		1 // filedrop on

// mouse move
#if QTB_EXTRA_BUTTON_SUPPORT
typedef char	MOUSE_MOVE;
#define MOUSE_MOVE_OFF		0	// not moving mouse
#define MOUSE_MOVE_ON		1	// moving mouse
#else // !QTB_EXTRA_BUTTON_SUPPORT
typedef long	MOUSE_MOVE;
#define MOUSE_MOVE_OFF		0L	// not moving mouse
#define MOUSE_MOVE_ON		1L	// moving mouse
#endif // !QTB_EXTRA_BUTTON_SUPPORT

// mouse button
typedef char	MOUSE_BUTTON;
#define MOUSE_BUTTON_DOWN	1	// button down
#define MOUSE_BUTTON_UP		2	// button up
#define MOUSE_BUTTON_DBLCLK	3	// button double click

// mouse wheel
typedef char	MOUSE_WHEEL;

// keycode flag
typedef	unsigned char	KEYCODE_FLG;
#define KEYCODE_FLG_KEYUP	0x00	// key up
#define KEYCODE_FLG_KEYDOWN	0x80	// key down

// monitor no
typedef	char	MONITOR_NO;

// monitor count
typedef	char	MONITOR_COUNT;

// key down
typedef long	KEYDOWN;
#define KEYDOWN_OFF	0L	// no key down
#define KEYDOWN_ON	1L	// key down

// sound capture
typedef char	SOUND_CAPTURE;
#define	SOUND_CAPTURE_DIRECT_SOUND	0	// Direct Sound
#define	SOUND_CAPTURE_CORE_AUDIO	1	// Core Audio

// video mode
typedef char VIDEO_MODE;
#define VIDEO_MODE_SPEED			0	// Speed      (GBVC) : NOT supoort by public mode
#define VIDEO_MODE_HYBRID			1	// Hybrid     (GBMJ) : NOT supoort by public mode
#define VIDEO_MODE_MJPEG			2	// MotionJPEG
#define VIDEO_MODE_COMPRESS			3	// Compress   (VP8)

// video quality
typedef char	VIDEO_QUALITY;
#define VIDEO_QUALITY_MINIMUM		1	// minimum quality
#define VIDEO_QUALITY_LOW			3	// low quality
#define VIDEO_QUALITY_STANDARD		5	// standard quality
#define VIDEO_QUALITY_HIGH			7	// high quality
#define VIDEO_QUALITY_MAXIMUM		9	// maximum quality

// mouse cursor
typedef char	MOUSE_CURSOR;
#define MOUSE_CURSOR_AUTO			0	// auto
#define MOUSE_CURSOR_ON				1	// display mouse cursor
#define MOUSE_CURSOR_OFF			2	// undisplay mouse cursor

// gamepad value
typedef long	GAMEPAD_VALUE;

// mode
typedef	char	MODE;
#define MODE_PUBLIC5					5	// public mode 5
#define MODE_PUBLIC6					6	// public mode 6
#define MODE_PUBLIC7					7	// public mode 7
// result value
#define MODE_RESULT_PASSWORD_ERROR		 0	// password error
#define MODE_RESULT_CONNECT_ERROR		-1	// connect error
#define MODE_RESULT_CONNECT_WAIT		-2	// connect wait

// sound quality
typedef	char	SOUND_QUALITY;
#define SOUND_QUALITY_MINIMUM	1	// minimum quality
#define SOUND_QUALITY_LOW		2	// low quality
#define SOUND_QUALITY_STANDARD	3	// standard quality
#define SOUND_QUALITY_HIGH		4	// high quality
#define SOUND_QUALITY_MAXIMUM	5	// maximum quality

// position
typedef	long	POS;
// size
typedef	long	SIZE;
// size 64bit
typedef	long long	SIZE64;
// zoom
typedef double	ZOOM;

// Protocol Header Structure (256 byte fixed)
#include "protocols_common.h"

} // end of namespace qtbrynhildr

#endif /* PROTOCOLS_x86_H */
