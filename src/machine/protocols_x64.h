// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// protocols_x64.h

#ifndef PROTOCOLS_x64_H
#define PROTOCOLS_x64_H
// Common Header
#include "common/common.h"

namespace qtbrynhildr {

// Protocol Header for PUBLIC MODE of Brynhildr
// for x64 architecture

#if QTB_BRYNHILDR2_SUPPORT
// server version
typedef int	SERVER_VERSION;
#define SERVER_VERSION_BRYNHILDR	1	// brynhildr
#define SERVER_VERSION_BRYNHILDR2	2	// brynhildr 2
#endif // QTB_BRYNHILDR2_SUPPORT

// data type
typedef char	DATA_TYPE;
#define DATA_TYPE_DATA		1	// fixed value

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
typedef int		CHECK_DIGIT_VALUE;

// sample rate
typedef int		SAMPLERATE;

// control
typedef int		CONTROL;	
#define CONTROL_OFF			0	// control off
#define CONTROL_ON			1	// control on

// mouse move
typedef int		MOUSE_MOVE;
#define MOUSE_MOVE_OFF		0	// not moving mouse
#define MOUSE_MOVE_ON		1	// moving mouse

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
typedef int	KEYDOWN;
#define KEYDOWN_OFF	0	// no key down
#define KEYDOWN_ON	1	// key down

// sound capture
typedef int		SOUND_CAPTURE;
#define	SOUND_CAPTURE_DIRECT_SOUND	0	// Direct Sound
#define	SOUND_CAPTURE_CORE_AUDIO	1	// Core Audio

// video quality
typedef int		VIDEO_QUALITY;
#define VIDEO_QUALITY_MINIMUM		1	// minimum quality
#define VIDEO_QUALITY_LOW			3	// low quality
#define VIDEO_QUALITY_STANDARD		5	// standard quality
#define VIDEO_QUALITY_HIGH			7	// high quality
#define VIDEO_QUALITY_MAXIMUM		9	// maximum quality

#if QTB_BRYNHILDR2_SUPPORT
// mouse cursor
typedef int	MOUSE_CURSOR;
#define MOUSE_CURSOR_AUTO			0	// auto
#define MOUSE_CURSOR_ON				1	// display mouse cursor
#define MOUSE_CURSOR_OFF			2	// undisplay mouse cursor

// gamepad value
typedef int	GAMEPAD_VALUE;
#endif // QTB_BRYNHILDR2_SUPPORT

// mode
typedef	int		MODE;
#define MODE_PUBLIC						5	// public mode
// result value
#define MODE_RESULT_PASSWORD_ERROR		0	// password error
#define MODE_RESULT_CONNECT_ERROR		-1	// connect error
#define MODE_RESULT_CONNECT_WAIT		-2	// connect wait

// sound quality
typedef	int		SOUND_QUALITY;
#define SOUND_QUALITY_MINIMUM	1	// minimum quality
#define SOUND_QUALITY_LOW		2	// low quality
#define SOUND_QUALITY_STANDARD	3	// standard quality
#define SOUND_QUALITY_HIGH		4	// high quality
#define SOUND_QUALITY_MAXIMUM	5	// maximum quality

// position
typedef	int		POS;
// size
typedef	int		SIZE;
// zoom
typedef double	ZOOM;

// etc
typedef char		BYTE;	// 1 byte
typedef short int	SHORT;	// 2 byte
typedef int			WORD;	// 4 byte
typedef long		DWORD;	// 8 byte

// Protocol Header Structure (256 byte fixed)
#include "protocols_common.h"

} // end of namespace qtbrynhildr

#endif /* PROTOCOLS_x64_H */
