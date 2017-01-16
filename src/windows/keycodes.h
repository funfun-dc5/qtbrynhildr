// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYCODES_H
#define KEYCODES_H

// virtual keycode for windows

// 00 - 0F
#define VK_NONE_00		0x00
#define VK_LBUTTON		0x01	// mouse left button
#define VK_RBUTTON		0x02	// mouse right button
#define VK_CANCEL		0x03
#define VK_MBUTTON		0x04	// mouse middle button
#define VK_XBUTTON1		0x05
#define VK_XBUTTON2		0x06
#define VK_NONE_07		0x07
#define VK_BACK			0x08	// BackSpace
#define VK_TAB			0x09	// Tab
#define VK_NONE_0A		0x0A
#define VK_NONE_0B		0x0B
#define VK_CLEAR		0x0C	// Clear
#define VK_RETURN		0x0D	// Enter
#define VK_NONE_0E		0x0E
#define VK_NONE_0F		0x0F


// 10 - 1F
#define VK_SHIFT		0x10	// Shift
#define VK_CONTROL		0x11	// Ctrl
#define VK_MENU			0x12	// Alt
#define VK_PAUSE		0x13	// Pause
#define VK_CAPITAL		0x14	// CapsLock
#define VK_KANA			0x15	// Kana (same as Hangul)
#define VK_HANGUL		0x15	// Hangul (same as Kana)
#define VK_NONE_16		0x16
#define VK_JUNJA		0x17
#define VK_FINAL		0x18
#define VK_KANJI		0x19	// Kanji (same as Hanja)
#define VK_HANJA		0x19	// Hanja (same as Kanji)
#define VK_NONE_1A		0x1A
#define VK_ESCAPE		0x1B	// Esc
#define VK_CONVERT		0x1C	// Convert
#define VK_NONCONVERT	0x1D	// NonConvert
#define VK_ACCEPT		0x1E
#define VK_MODECHANGE	0x1F

// 20 - 2F
#define VK_SPACE		0x20	// Space
#define VK_PRIOR		0x21	// PageUp
#define VK_NEXT			0x22	// PageDown
#define VK_END			0x23	// End
#define VK_HOME			0x24	// Home
#define VK_LEFT			0x25
#define VK_UP			0x26
#define VK_RIGHT		0x27
#define VK_DOWN			0x28
#define VK_SELECT		0x29	// Select
#define VK_NONE_2A		0x2A
#define VK_EXECUTE		0x2B	// Execute
#define VK_SNAPSHOT		0x2C	// PrintScreen
#define VK_INSERT		0x2D	// Ins
#define VK_DELETE		0x2E	// Del
#define VK_HELP			0x2F	// Help

// 30 - 3F
#define VK_0			0x30	// 0
#define VK_1			0x31	// 1
#define VK_2			0x32	// 2
#define VK_3			0x33	// 3
#define VK_4			0x34	// 4
#define VK_5			0x35	// 5
#define VK_6			0x36	// 6
#define VK_7			0x37	// 7
#define VK_8			0x38	// 8
#define VK_9			0x39	// 9
#define VK_NONE_3A		0x3A
#define VK_NONE_3B		0x3B
#define VK_NONE_3C		0x3C
#define VK_NONE_3D		0x3D
#define VK_NONE_3E		0x3E
#define VK_NONE_3F		0x3F

// 40 - 4F
#define VK_NONE_40		0x40
#define VK_A			0x41	// A
#define VK_B			0x42	// B
#define VK_C			0x43	// C
#define VK_D			0x44	// D
#define VK_E			0x45	// E
#define VK_F			0x46	// F
#define VK_G			0x47	// G
#define VK_H			0x48	// H
#define VK_I			0x49	// I
#define VK_J			0x4A	// J
#define VK_K			0x4B	// K
#define VK_L			0x4C	// L
#define VK_M			0x4D	// M
#define VK_N			0x4E	// N
#define VK_O			0x4F	// O

// 50 - 5F
#define VK_P			0x50	// P
#define VK_Q			0x51	// Q
#define VK_R			0x52	// R
#define VK_S			0x53	// S
#define VK_T			0x54	// T
#define VK_U			0x55	// U
#define VK_V			0x56	// V
#define VK_W			0x57	// W
#define VK_X			0x58	// X
#define VK_Y			0x59	// Y
#define VK_Z			0x5A	// Z
#define VK_LWIN			0x5B	// left Windows
#define VK_RWIN			0x5C	// right Windows
#define VK_APPS			0x5D	// Application
#define VK_NONE_5E		0x5E
#define VK_SLEEP		0x5F

// 60 - 6F
#define VK_NUMPAD0		0x60	// Num 0
#define VK_NUMPAD1		0x61	// Num 1
#define VK_NUMPAD2		0x62	// Num 2
#define VK_NUMPAD3		0x63	// Num 3
#define VK_NUMPAD4		0x64	// Num 4
#define VK_NUMPAD5		0x65	// Num 5
#define VK_NUMPAD6		0x66	// Num 6
#define VK_NUMPAD7		0x67	// Num 7
#define VK_NUMPAD8		0x68	// Num 8
#define VK_NUMPAD9		0x69	// Num 9
#define VK_MULTIPLY		0x6A	// Num *
#define VK_ADD			0x6B	// Num +
#define VK_SEPARATOR	0x6C	// Num ,
#define VK_SUBTRACT		0x6D	// Num -
#define VK_DECIMAL		0x6E	// Num .
#define VK_DIVIDE		0x6F	// Num /

// 70 - 7F
#define VK_F1			0x70	// F1
#define VK_F2			0x71	// F2
#define VK_F3			0x72	// F3
#define VK_F4			0x73	// F4
#define VK_F5			0x74	// F5
#define VK_F6			0x75	// F6
#define VK_F7			0x76	// F7
#define VK_F8			0x77	// F8
#define VK_F9			0x78	// F9
#define VK_F10			0x79	// F10
#define VK_F11			0x7A	// F11
#define VK_F12			0x7B	// F12
#define VK_F13			0x7C	// F13
#define VK_F14			0x7D	// F14
#define VK_F15			0x7E	// F15
#define VK_F16			0x7F	// F16

// 80 - 8F
#define VK_F17			0x80	// F17
#define VK_F18			0x81	// F18
#define VK_F19			0x82	// F19
#define VK_F20			0x83	// F20
#define VK_F21			0x84	// F21
#define VK_F22			0x85	// F22
#define VK_F23			0x86	// F23
#define VK_F24			0x87	// F24
#define VK_NONE_88		0x88
#define VK_NONE_89		0x89
#define VK_NONE_8A		0x8A
#define VK_NONE_8B		0x8B
#define VK_NONE_8C		0x8C
#define VK_NONE_8D		0x8D
#define VK_NONE_8E		0x8E
#define VK_NONE_8F		0x8F

// 90 - 9F
#define VK_NUMLOCK		0x90	// NumLock
#define VK_SCROLL		0x91	// ScrollLock
#define VK_OEM_EQU		0x92	// OEM Num =
#define VK_NONE_93		0x93
#define VK_NONE_94		0x94
#define VK_NONE_95		0x95
#define VK_NONE_96		0x96
#define VK_NONE_97		0x97
#define VK_NONE_98		0x98
#define VK_NONE_99		0x99
#define VK_NONE_9A		0x9A
#define VK_NONE_9B		0x9B
#define VK_NONE_9C		0x9C
#define VK_NONE_9D		0x9D
#define VK_NONE_9E		0x9E
#define VK_NONE_9F		0x9F

// A0 - AF
#define VK_LSHIFT		0xA0	// left Shift
#define VK_RSHIFT		0xA1	// right Shift
#define VK_LCONTROL		0xA2	// left Ctrl
#define VK_RCONTROL		0xA3	// right Ctrl
#define VK_LMENU		0xA4	// left Alt
#define VK_RMENU		0xA5	// right Alt
#define VK_BROWSER_BACK			0xA6
#define VK_BROWSER_FORWARD		0xA7
#define VK_BROWSER_REFRESH		0xA8
#define VK_BROWSER_STOP			0xA9
#define VK_BROWSER_SEARCH		0xAA
#define VK_BROWSER_FAVORITES	0xAB
#define VK_BROWSER_HOME			0xAC
#define VK_VOLUME_MUTE			0xAD
#define VK_VOLUME_DOWN			0xAE
#define VK_VOLUME_UP			0xAF

// B0 - BF
#define VK_MEDIA_NEXT_TRACK		0xB0
#define VK_MEDIA_PREV_TRACK		0xB1
#define VK_MEDIA_STOP			0xB2
#define VK_MEDIA_PLAY_PAUSE		0xB3
#define VK_LAUNCH_MAIL			0xB4
#define VK_LAUNCH_MEDIA_SELECT	0xB5
#define VK_LAUNCH_APP1			0xB6
#define VK_LAUNCH_APP2			0xB7
#define VK_NONE_B8		0xB8
#define VK_NONE_B9		0xB9
#define VK_OEM_1		0xBA	// OEM	':'
#define VK_OEM_PLUS		0xBB	// OEM	';'
#define VK_OEM_COMMA	0xBC	// OEM	','
#define VK_OEM_MINUS	0xBD	// OEM	'-'
#define VK_OEM_PERIOD	0xBE	// OEM	'.'
#define VK_OEM_2		0xBF	// OEM	'/'
#define VK_OEM_3		0xC0	// OEM	'@'

// C0 - CF
#define VK_NONE_C1		0xC1
#define VK_NONE_C2		0xC2
#define VK_NONE_C3		0xC3
#define VK_NONE_C4		0xC4
#define VK_NONE_C5		0xC5
#define VK_NONE_C6		0xC6
#define VK_NONE_C7		0xC7
#define VK_NONE_C8		0xC8
#define VK_NONE_C9		0xC9
#define VK_NONE_CA		0xCA
#define VK_NONE_CB		0xCB
#define VK_NONE_CC		0xCC
#define VK_NONE_CD		0xCD
#define VK_NONE_CE		0xCE
#define VK_NONE_CF		0xCF

// D0 - DF
#define VK_NONE_D0		0xD0
#define VK_NONE_D1		0xD1
#define VK_NONE_D2		0xD2
#define VK_NONE_D3		0xD3
#define VK_NONE_D4		0xD4
#define VK_NONE_D5		0xD5
#define VK_NONE_D6		0xD6
#define VK_NONE_D7		0xD7
#define VK_NONE_D8		0xD8
#define VK_NONE_D9		0xD9
#define VK_NONE_DA		0xDA
#define VK_OEM_4		0xDB	// OEM	'['
#define VK_OEM_5		0xDC	// OEM	'\'
#define VK_OEM_6		0xDD	// OEM	']'
#define VK_OEM_7		0xDE	// OEM	'^'
#define VK_OEM_8		0xDF	// OEM	'_'

// E0 - EF
#define VK_NONE_E0		0xE0
#define VK_OEM_AX		0xE1
#define VK_OEM_102		0xE2	// '_'
#define VK_ICO_HELP		0xE3
#define VK_ICO_00		0xE4
#define VK_PROCESSKEY	0xE5
#define VK_ICO_CLEAR	0xE6
#define VK_PACKET		0xE7
#define VK_NONE_E8		0xE8
#define VK_OEM_RESET	0xE9
#define VK_OEM_JUMP		0xEA
#define VK_OEM_PA1		0xEB
#define VK_OEM_PA2		0xEC
#define VK_OEM_PA3		0xED
#define VK_OEM_WSCTRL	0xEE
#define VK_OEM_CUSEL	0xEF

// F0 - FF
#define VK_OEM_ATTN		0xF0
#define VK_OEM_FINISH	0xF1
#define VK_OEM_COPY		0xF2
#define VK_OEM_AUTO		0xF3
#define VK_OEM_ENLW		0xF4
#define VK_OEM_BACKTAB	0xF5
#define VK_ATTN			0xF6
#define VK_CRSEL		0xF7
#define VK_EXSEL		0xF8
#define VK_EREOF		0xF9
#define VK_PLAY			0xFA
#define VK_ZOOM			0xFB
#define VK_NONAME		0xFC
#define VK_PA1			0xFD
#define VK_OEM_CLEAR	0xFE
#define VK_NONE_FF		0xFF

#endif // KEYCODES_H
