// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015-2018 FunFun <fu.aba.dc5@gmail.com>

#ifndef PROTOCOLS_common_H
#define PROTOCOLS_common_H
// Common Header
#include "common/common.h"

// etc
typedef char		BYTE;	// 1 byte
typedef short int	SHORT;	// 2 byte
typedef int			WORD;	// 4 byte

// Protocol Header Structure (256 byte fixed)
#if _MSC_VER
#pragma pack(push, 1)
typedef struct COM_DATA_BASE { // for C/C++ Compatibility
#else // _MSC_VER
typedef struct __attribute__((__packed__)) COM_DATA_BASE { // for C/C++ Compatibility
#endif // _MSC_VER
  DATA_TYPE			data_type;
  THREAD			thread;
  BYTE				dummy1[1];			// padding 1 byte
  SOUND_TYPE		sound_type;
  ENCRYPTION		encryption;
  BYTE				dummy2[3];			// padding 3 bytes
  SIZE64			data_size;
  CHECK_DIGIT		check_digit_enc[ENCRYPTION_KEY_LENGTH];
  CHECK_DIGIT_VALUE	check_digit;
  char				ver[PROTOCOL_VERSION_STRING_LENGTH];
  SAMPLERATE		samplerate;
  SIZE				image_cx;			// width of image
  SIZE				image_cy;			// height of image
  SIZE				server_cx;			// width of image for server.
  SIZE				server_cy;			// height of image for server.
  CONTROL			control;
#if QTB_PLUGINS_DISABLE_SUPPORT
  BYTE				dummy3[1];			// padding 1 bytes
  BYTE				plugins_disable;	// disable plugins
#else // QTB_PLUGINS_DISABLE_SUPPORT
  BYTE				dummy3[2];			// padding 2 bytes
#endif // QTB_PLUGINS_DISABLE_SUPPORT
  BYTE				filedrop;			// file drop from server
#if QTB_EXTRA_BUTTON_SUPPORT
  MOUSE_MOVE		mouse_move;			// 1 byte
  BYTE				dummy4[1];
  MOUSE_BUTTON		mouse_x1;			// mouse button status for back button
  MOUSE_BUTTON		mouse_x2;			// mouse button status for forward button
#else // QTB_EXTRA_BUTTON_SUPPORT
  MOUSE_MOVE		mouse_move;			// 4 bytes
#endif // QTB_EXTRA_BUTTON_SUPPORT
  POS			    mouse_x;			// mouse position x
  POS			    mouse_y;			// mouse position y
  MOUSE_BUTTON		mouse_left;			// mouse button status for left button
  MOUSE_BUTTON		mouse_right;		// mouse button status for right button
#if QTB_EXTRA_BUTTON_SUPPORT
  MOUSE_BUTTON		mouse_middle;		// mouse button status for middle button
#else // QTB_EXTRA_BUTTON_SUPPORT
  BYTE				dummy4[1];			// padding 1 byte
#endif // QTB_EXTRA_BUTTON_SUPPORT
  MOUSE_WHEEL		mouse_wheel;
  char				keycode;
  KEYCODE_FLG		keycode_flg;
  char				cursor_hotspot_x;	// cursor hotspot x
  char				cursor_hotspot_y;	// cursor hotspot y
  MONITOR_NO		monitor_no;
  MONITOR_COUNT		monitor_count;
  BYTE				dummy6[2];			// padding 2 bytes
  char				max_fps;			// max FPS
  BYTE				dummy16[3];			// padding 3 bytes
  SOUND_CAPTURE		sound_capture;
  BYTE				dummy7[3];			// padding 3 bytes
  BYTE				dummy8[40];			// padding 40 bytes
  KEYDOWN			keydown;
  VIDEO_QUALITY		video_quality;
  BYTE				dummy9[3];			// padding 6 bytes
  MOUSE_CURSOR		mouse_cursor;		// mouse cursor
  BYTE				dummy10[23];		// padding 23 bytes
  GAMEPAD_VALUE		gamepad1;			// gamepad (Xpos)
  GAMEPAD_VALUE		gamepad2;			// gamepad (Ypos)
  GAMEPAD_VALUE		gamepad3;			// gamepad (Zpos)
  GAMEPAD_VALUE		gamepad4;			// gamepad (Rpos)
  POS				client_scroll_x;
  POS				client_scroll_y;
  VIDEO_MODE		video_mode;
  BYTE				dummy11[3];			// padding 3 bytes
  char				scroll;				// scroll enable flag (public mode 7 only)
  BYTE				dummy17[15];		// padding 15 bytes
  SERVER_VERSION	server_version;		// version of server
  ZOOM				zoom;
  BYTE				dummy12[4];			// padding 4 bytes
  MODE				mode;
  BYTE				dummy13[3];			// padding 3 bytes
  SOUND_QUALITY		sound_quality;
  BYTE				dummy14[3];			// padding 3 bytes
  unsigned char		frame_no;			// frame number
  BYTE				dummy15[3];			// padding 3 bytes
  GAMEPAD_VALUE		gamepad5;			// gamepad (Pov)
  GAMEPAD_VALUE		gamepad6;			// gamepad (Buttons)
  GAMEPAD_VALUE		gamepad7;			// gamepad (Upos)
  GAMEPAD_VALUE		gamepad8;			// gamepad (Vpos)
} COM_DATA;
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

#endif /* PROTOCOLS_common_H */
