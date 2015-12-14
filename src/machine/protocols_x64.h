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

#if defined(QTB_BRYNHILDR2_SUPPORT)
// server version
typedef int	SERVER_VERSION;
#define SERVER_VERSION_BRYNHILDR	1	// brynhildr
#define SERVER_VERSION_BRYNHILDR2	2	// brynhildr 2
#endif // defined(QTB_BRYNHILDR2_SUPPORT)

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
typedef int		ENCRYPTION;
#define ENCRYPTION_OFF		0	// encryption OFF
#define ENCRYPTION_ON		1	// encryption ON

// check digit
typedef char	CHECK_DIGIT;

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

#if defined(QTB_BRYNHILDR2_SUPPORT)
// mouse cursor
typedef int	MOUSE_CURSOR;
#define MOUSE_CURSOR_AUTO			0	// auto
#define MOUSE_CURSOR_ON				1	// display mouse cursor
#define MOUSE_CURSOR_OFF			2	// undisplay mouse cursor
#endif // defined(QTB_BRYNHILDR2_SUPPORT)

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
typedef struct __attribute__((__packed__)) COM_DATA_BASE { // for C/C++ Compatibility
  DATA_TYPE			data_type;			// fixed(DATA_TYPE_DATA)
  THREAD			thread;
  BYTE				dummy1[1];			// padding 1 byte
  SOUND_TYPE		sound_type;
  ENCRYPTION		encryption;
  SIZE				data_size;
  BYTE				dummy2[4];			// padding 4 bytes
  CHECK_DIGIT		check_digit_enc[ENCRYPTION_KEY_LENGTH];
  int				check_digit;
  char				ver[PROTOCOL_VERSION_STRING_LENGTH];
  SAMPLERATE		samplerate;
  SIZE				image_cx;			// width of image
  SIZE				image_cy;			// hight of image
  SIZE				server_cx;			// width of image for server.
  SIZE				server_cy;			// hight of image for server.
  CONTROL			control;			// fixed(CONTROL_ON)
  MOUSE_MOVE		mouse_move;
  POS			    mouse_x;			// mouse position x
  POS			    mouse_y;			// mouse position y
  MOUSE_BUTTON		mouse_left;			// mouse button status for left button
  MOUSE_BUTTON		mouse_right;		// mouse button status for right button
  BYTE				dummy3[1];			// padding 1 byte
  MOUSE_WHEEL		mouse_wheel;
  char				keycode;
  KEYCODE_FLG		keycode_flg;
  BYTE				dummy4[2];			// padding 2 bytes
  MONITOR_NO		monitor_no;
  MONITOR_COUNT		monitor_count;
  BYTE				dummy5[6];			// padding 6 bytes
  SOUND_CAPTURE		sound_capture;
  BYTE				dummy6[40];			// padding 40 bytes
  KEYDOWN			keydown;
  VIDEO_QUALITY		video_quality;
#if defined(QTB_BRYNHILDR2_SUPPORT)
  MOUSE_CURSOR		mouse_cursor;		// mouse cursor
  BYTE				dummy7[36];			// padding 36 bytes
#else // defined(QTB_BRYNHILDR2_SUPPORT)
  BYTE				dummy7[40];			// padding 40 bytes
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  POS				client_scroll_x;
  POS				client_scroll_y;

#if defined(QTB_BRYNHILDR2_SUPPORT)
  BYTE				dummy8[20];			// padding 20 bytes
  SERVER_VERSION	server_version;		// version of server
#else // defined(QTB_BRYNHILDR2_SUPPORT)
  BYTE				dummy8[24];			// padding 24 bytes
#endif // defined(QTB_BRYNHILDR2_SUPPORT)
  ZOOM				zoom;
  BYTE				dummy9[4];			// padding 4 bytes
  MODE				mode;
  SOUND_QUALITY		sound_quality;
  BYTE				dummy10[20];
} COM_DATA;

} // end of namespace qtbrynhildr

#endif /* PROTOCOLS_x64_H */
