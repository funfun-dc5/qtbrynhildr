// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// protocols_x86.h

#ifndef PROTOCOLS_x86_H
#define PROTOCOLS_x86_H
// Common Header
#include "common/common.h"

namespace qtbrynhildr {

// Protocol Header for PUBLIC MODE of Brynhildr
// for x86 architecture

#if QTB_BRYNHILDR2_SUPPORT
// server version
typedef long	SERVER_VERSION;
#define SERVER_VERSION_BRYNHILDR	1	// brynhildr
#define SERVER_VERSION_BRYNHILDR2	2	// brynhildr 2
#endif // QTB_BRYNHILDR2_SUPPORT

// data type
typedef char	DATA_TYPE;
#define DATA_TYPE_DATA		1	// data
#if QTB_PUBLIC_MODE6_SUPPORT
#define DATA_TYPE_CLIPBOARD	2	// clipboard
#define DATA_TYPE_FILE		3	// file
#endif // QTB_PUBLIC_MODE6_SUPPORT

// thread
typedef char	THREAD;
#define THREAD_CONTROL		1	// thread for control
#define THREAD_GRAPHICS		2	// thread for graphics
#define THREAD_SOUND		3	// thread for sound

// sound type
typedef	char	SOUND_TYPE;
#define SOUND_TYPE_OFF		0	// sound OFF
#define SOUNT_TYPE_ON		1	// sound ON

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
typedef long	CONTROL;	
#define CONTROL_OFF			0L	// control off
#define CONTROL_ON			1L	// control on

// mouse move
#if QTB_EXTRA_BUTTON_SUPPORT
typedef char	MOUSE_MOVE;
#define MOUSE_MOVE_OFF		0	// not moving mouse
#define MOUSE_MOVE_ON		1	// moving mouse
#else // QTB_EXTRA_BUTTON_SUPPORT
typedef long	MOUSE_MOVE;
#define MOUSE_MOVE_OFF		0L	// not moving mouse
#define MOUSE_MOVE_ON		1L	// moving mouse
#endif // QTB_EXTRA_BUTTON_SUPPORT

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
typedef long	SOUND_CAPTURE;
#define	SOUND_CAPTURE_DIRECT_SOUND	0L	// Direct Sound
#define	SOUND_CAPTURE_CORE_AUDIO	1L	// Core Audio

// video quality
typedef long	VIDEO_QUALITY;
#define VIDEO_QUALITY_MINIMUM		1L	// minimum quality
#define VIDEO_QUALITY_LOW			3L	// low quality
#define VIDEO_QUALITY_STANDARD		5L	// standard quality
#define VIDEO_QUALITY_HIGH			7L	// high quality
#define VIDEO_QUALITY_MAXIMUM		9L	// maximum quality

#if QTB_BRYNHILDR2_SUPPORT
// mouse cursor
typedef long	MOUSE_CURSOR;
#define MOUSE_CURSOR_AUTO			0L	// auto
#define MOUSE_CURSOR_ON				1L	// display mouse cursor
#define MOUSE_CURSOR_OFF			2L	// undisplay mouse cursor

// gamepad value
typedef long	GAMEPAD_VALUE;
#endif // QTB_BRYNHILDR2_SUPPORT

// mode
typedef	long	MODE;
#define MODE_PUBLIC5					5L	// public mode 5
#if QTB_PUBLIC_MODE6_SUPPORT
#define MODE_PUBLIC6					6L	// public mode 6
#endif // QTB_PUBLIC_MODE6_SUPPORT
// result value
#define MODE_RESULT_PASSWORD_ERROR		0L	// password error
#define MODE_RESULT_CONNECT_ERROR		-1L	// connect error
#define MODE_RESULT_CONNECT_WAIT		-2L	// connect wait

// sound quality
typedef	long	SOUND_QUALITY;
#define SOUND_QUALITY_MINIMUM	1L	// minimum quality
#define SOUND_QUALITY_LOW		2L	// low quality
#define SOUND_QUALITY_STANDARD	3L	// standard quality
#define SOUND_QUALITY_HIGH		4L	// high quality
#define SOUND_QUALITY_MAXIMUM	5L	// maximum quality

// position
typedef	long	POS;
// size
typedef	long	SIZE;
// zoom
typedef double	ZOOM;

// etc
typedef char		BYTE;	// 1 byte
typedef short int	SHORT;	// 2 byte
typedef int			WORD;	// 4 byte

// Protocol Header Structure (256 byte fixed)
#include "protocols_common.h"

} // end of namespace qtbrynhildr

#endif /* PROTOCOLS_x86_H */
