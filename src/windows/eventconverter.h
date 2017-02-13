// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef EVENTCONVERTER_H
#define EVENTCONVERTER_H

// Qt Header
#include <QKeyEvent>
#include <QString>
#include <Qt>

// Local Header
#include "keycodes.h"

using namespace Qt;

namespace qtbrynhildr {

// EventConverter
class EventConverter
{
public:
  // keyboard type
  typedef enum {
	KEYTOP_TYPE_JP,
	KEYTOP_TYPE_US
  } KEYTOP_TYPE;

  // shift key control
  typedef enum {
	SHIFTKEY_THROUGH,
	SHIFTKEY_NEED,
	SHIFTKEY_NONEED,
  } ShiftKeyControl;

public:
  // constructor
  EventConverter();
  EventConverter(KEYTOP_TYPE type);

#if 1 // for TEST
  // get keytop type
  KEYTOP_TYPE getKeytopType();

  // set keytop type
  void setKeytopType(KEYTOP_TYPE type);
#endif

  // get Virtual Keycode
  virtual uchar getVKCode(QKeyEvent *keyEvent);

  // get shift key control
  ShiftKeyControl getShiftKeyControl();

  // get name
  virtual QString getEventConverterName();

  // for DEBUG
  static QString getVKCodeByString(uchar vkcode);

protected:
  // key table size
  static const int TABLE_SIZE = 139;

  // key table for event convert
  typedef struct {
	Key key;
	uchar VK_Code;
	ShiftKeyControl shiftKeyControl;
  } KeyEvent;

  // key table
  const KeyEvent *keyEventInfo;

  // key table for JP
  const KeyEvent keyEventInfo_JP[TABLE_SIZE] = {
	{Key_Escape,				VK_ESCAPE,		SHIFTKEY_THROUGH},
	{Key_Tab,					VK_TAB,			SHIFTKEY_THROUGH},
	{Key_Backspace,				VK_BACK,		SHIFTKEY_THROUGH},
	{Key_Return,				VK_RETURN,		SHIFTKEY_THROUGH},
	{Key_Enter,					VK_RETURN,		SHIFTKEY_THROUGH},
	{Key_Insert,				VK_INSERT,		SHIFTKEY_THROUGH},
	{Key_Delete,				VK_DELETE,		SHIFTKEY_THROUGH},
	{Key_Pause,					VK_PAUSE,		SHIFTKEY_THROUGH},
	{Key_Print,					VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Key_SysReq,				VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Key_Clear,					VK_CLEAR,		SHIFTKEY_THROUGH},
	{Key_Home,					VK_HOME,		SHIFTKEY_THROUGH},
	{Key_End,					VK_END,			SHIFTKEY_THROUGH},
	{Key_Left,					VK_LEFT,		SHIFTKEY_THROUGH},
	{Key_Up,					VK_UP,			SHIFTKEY_THROUGH},
	{Key_Right,					VK_RIGHT,		SHIFTKEY_THROUGH},
	{Key_Down,					VK_DOWN,		SHIFTKEY_THROUGH},
	{Key_PageUp,				VK_PRIOR,		SHIFTKEY_THROUGH},
	{Key_PageDown,				VK_NEXT,		SHIFTKEY_THROUGH},
	{Key_Shift,					VK_SHIFT,		SHIFTKEY_THROUGH},
#if defined(Q_OS_OSX)
	{Key_Control,				VK_LWIN,		SHIFTKEY_THROUGH},
	{Key_Meta,					VK_CONTROL,		SHIFTKEY_THROUGH},
#else // defined(Q_OS_OSX)
	{Key_Control,				VK_CONTROL,		SHIFTKEY_THROUGH},
	{Key_Meta,					VK_LWIN,		SHIFTKEY_THROUGH},
#endif // defined(Q_OS_OSX)
	{Key_Alt,					VK_MENU,		SHIFTKEY_THROUGH},
	{Key_CapsLock,				VK_CAPITAL,		SHIFTKEY_THROUGH},
	{Key_NumLock,				VK_NUMLOCK,		SHIFTKEY_THROUGH},
	{Key_ScrollLock,			VK_SCROLL,		SHIFTKEY_THROUGH},

	{Key_F1,					VK_F1,			SHIFTKEY_THROUGH},
	{Key_F2,					VK_F2,			SHIFTKEY_THROUGH},
	{Key_F3,					VK_F3,			SHIFTKEY_THROUGH},
	{Key_F4,					VK_F4,			SHIFTKEY_THROUGH},
	{Key_F5,					VK_F5,			SHIFTKEY_THROUGH},
	{Key_F6,					VK_F6,			SHIFTKEY_THROUGH},
	{Key_F7,					VK_F7,			SHIFTKEY_THROUGH},
	{Key_F8,					VK_F8,			SHIFTKEY_THROUGH},
	{Key_F9,					VK_F9,			SHIFTKEY_THROUGH},
	{Key_F10,					VK_F10,			SHIFTKEY_THROUGH},
	{Key_F11,					VK_F11,			SHIFTKEY_THROUGH},
	{Key_F12,					VK_F12,			SHIFTKEY_THROUGH},
	{Key_F13,					VK_F13,			SHIFTKEY_THROUGH},
	{Key_F14,					VK_F14,			SHIFTKEY_THROUGH},
	{Key_F15,					VK_F15,			SHIFTKEY_THROUGH},
	{Key_F16,					VK_F16,			SHIFTKEY_THROUGH},
	{Key_F17,					VK_F17,			SHIFTKEY_THROUGH},
	{Key_F18,					VK_F18,			SHIFTKEY_THROUGH},
	{Key_F19,					VK_F19,			SHIFTKEY_THROUGH},
	{Key_F20,					VK_F20,			SHIFTKEY_THROUGH},
	{Key_F21,					VK_F21,			SHIFTKEY_THROUGH},
	{Key_F22,					VK_F22,			SHIFTKEY_THROUGH},
	{Key_F23,					VK_F23,			SHIFTKEY_THROUGH},
	{Key_F24,					VK_F24,			SHIFTKEY_THROUGH},

	{Key_0,						VK_0,			SHIFTKEY_NONEED},
	{Key_1,						VK_1,			SHIFTKEY_NONEED},
	{Key_2,						VK_2,			SHIFTKEY_NONEED},
	{Key_3,						VK_3,			SHIFTKEY_NONEED},
	{Key_4,						VK_4,			SHIFTKEY_NONEED},
	{Key_5,						VK_5,			SHIFTKEY_NONEED},
	{Key_6,						VK_6,			SHIFTKEY_NONEED},
	{Key_7,						VK_7,			SHIFTKEY_NONEED},
	{Key_8,						VK_8,			SHIFTKEY_NONEED},
	{Key_9,						VK_9,			SHIFTKEY_NONEED},

	{Key_A,						VK_A,			SHIFTKEY_THROUGH},
	{Key_B,						VK_B,			SHIFTKEY_THROUGH},
	{Key_C,						VK_C,			SHIFTKEY_THROUGH},
	{Key_D,						VK_D,			SHIFTKEY_THROUGH},
	{Key_E,						VK_E,			SHIFTKEY_THROUGH},
	{Key_F,						VK_F,			SHIFTKEY_THROUGH},
	{Key_G,						VK_G,			SHIFTKEY_THROUGH},
	{Key_H,						VK_H,			SHIFTKEY_THROUGH},
	{Key_I,						VK_I,			SHIFTKEY_THROUGH},
	{Key_J,						VK_J,			SHIFTKEY_THROUGH},
	{Key_K,						VK_K,			SHIFTKEY_THROUGH},
	{Key_L,						VK_L,			SHIFTKEY_THROUGH},
	{Key_M,						VK_M,			SHIFTKEY_THROUGH},
	{Key_N,						VK_N,			SHIFTKEY_THROUGH},
	{Key_O,						VK_O,			SHIFTKEY_THROUGH},
	{Key_P,						VK_P,			SHIFTKEY_THROUGH},
	{Key_Q,						VK_Q,			SHIFTKEY_THROUGH},
	{Key_R,						VK_R,			SHIFTKEY_THROUGH},
	{Key_S,						VK_S,			SHIFTKEY_THROUGH},
	{Key_T,						VK_T,			SHIFTKEY_THROUGH},
	{Key_U,						VK_U,			SHIFTKEY_THROUGH},
	{Key_V,						VK_V,			SHIFTKEY_THROUGH},
	{Key_W,						VK_W,			SHIFTKEY_THROUGH},
	{Key_X,						VK_X,			SHIFTKEY_THROUGH},
	{Key_Y,						VK_Y,			SHIFTKEY_THROUGH},
	{Key_Z,						VK_Z,			SHIFTKEY_THROUGH},

	{Key_Space,					VK_SPACE,		SHIFTKEY_THROUGH},

	{Key_Exclam,				VK_1,			SHIFTKEY_NEED},		// '!'
	{Key_QuoteDbl,				VK_2,			SHIFTKEY_NEED},		// '"'
	{Key_NumberSign,			VK_3,			SHIFTKEY_NEED},		// '#'
	{Key_Dollar,				VK_4,			SHIFTKEY_NEED},		// '$'
	{Key_Percent,				VK_5,			SHIFTKEY_NEED},		// '%'
	{Key_Ampersand,				VK_6,			SHIFTKEY_NEED},		// '&'
	{Key_Apostrophe,			VK_7,			SHIFTKEY_NEED},		// '''
	{Key_ParenLeft,				VK_8,			SHIFTKEY_NEED},		// '('
	{Key_ParenRight,			VK_9,			SHIFTKEY_NEED},		// ')'

	{Key_Asterisk,				VK_OEM_1,		SHIFTKEY_NEED},		// '*'
	{Key_Plus,					VK_OEM_PLUS,	SHIFTKEY_NEED},		// '+'
	{Key_Comma,					VK_OEM_COMMA,	SHIFTKEY_NONEED},	// ','
	{Key_Minus,					VK_OEM_MINUS,	SHIFTKEY_NONEED},	// '-'
	{Key_Period,				VK_OEM_PERIOD,	SHIFTKEY_NONEED},	// '.'
	{Key_Slash,					VK_OEM_2,		SHIFTKEY_NONEED},	// '/'

	{Key_Colon,					VK_OEM_1,		SHIFTKEY_NONEED},	// ':'
	{Key_Semicolon,				VK_OEM_PLUS,	SHIFTKEY_NONEED},	// ';'
	{Key_Less,					VK_OEM_COMMA,	SHIFTKEY_NEED},		// '<'
	{Key_Equal,					VK_OEM_MINUS,	SHIFTKEY_NEED},		// '='
	{Key_Greater,				VK_OEM_PERIOD,	SHIFTKEY_NEED},		// '>'
	{Key_Question,				VK_OEM_2,		SHIFTKEY_NEED},		// '?'
	{Key_At,					VK_OEM_3,		SHIFTKEY_NONEED},	// '@'
	{Key_QuoteLeft,				VK_OEM_3,		SHIFTKEY_NEED},		// '`'

	{Key_BracketLeft,			VK_OEM_4,		SHIFTKEY_NONEED},	// '['
	{Key_BraceLeft,				VK_OEM_4,		SHIFTKEY_NEED},		// '{'
	{Key_Backslash,				VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Key_yen,					VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Key_Bar,					VK_OEM_5,		SHIFTKEY_NEED},		// '|'
	{Key_Underscore,			VK_OEM_102,		SHIFTKEY_NEED},		// '_'
	{Key_BracketRight,			VK_OEM_6,		SHIFTKEY_NONEED},	// ']'
	{Key_BraceRight,			VK_OEM_6,		SHIFTKEY_NEED},		// '}'
	{Key_AsciiTilde,			VK_OEM_7,		SHIFTKEY_NEED},		// '~'
	{Key_AsciiCircum,			VK_OEM_7,		SHIFTKEY_NONEED},	// '^'

	{Key_Henkan,				VK_CONVERT,		SHIFTKEY_THROUGH},	// Convert
	{Key_Muhenkan,				VK_NONCONVERT,	SHIFTKEY_THROUGH},	// NonConvert
	{Key_Zenkaku_Hankaku,		VK_KANJI,		SHIFTKEY_THROUGH},	// Zenkaku_Hankaku

	{Key_Back,					VK_BROWSER_BACK,		SHIFTKEY_THROUGH},
	{Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Key_Refresh,				VK_BROWSER_REFRESH,		SHIFTKEY_THROUGH},
	{Key_Stop,					VK_BROWSER_STOP,		SHIFTKEY_THROUGH},
	{Key_Search,				VK_BROWSER_SEARCH,		SHIFTKEY_THROUGH},
	{Key_Favorites,				VK_BROWSER_FAVORITES,	SHIFTKEY_THROUGH},
	{Key_HomePage,				VK_BROWSER_HOME,		SHIFTKEY_THROUGH},
	{Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Key_VolumeMute,			VK_VOLUME_MUTE,			SHIFTKEY_THROUGH},
	{Key_VolumeDown,			VK_VOLUME_DOWN,			SHIFTKEY_THROUGH},
	{Key_VolumeUp,				VK_VOLUME_UP,			SHIFTKEY_THROUGH},
	{Key_MediaNext,				VK_MEDIA_NEXT_TRACK,	SHIFTKEY_THROUGH},
	{Key_MediaPrevious,			VK_MEDIA_PREV_TRACK,	SHIFTKEY_THROUGH},
	{Key_MediaStop,				VK_MEDIA_STOP,			SHIFTKEY_THROUGH},
	{Key_MediaTogglePlayPause,	VK_MEDIA_PLAY_PAUSE,	SHIFTKEY_THROUGH},
	{Key_LaunchMail,			VK_LAUNCH_MAIL,			SHIFTKEY_THROUGH}
  };

  // key table for US
  const KeyEvent keyEventInfo_US[TABLE_SIZE] = {
	{Key_Escape,				VK_ESCAPE,		SHIFTKEY_THROUGH},
	{Key_Tab,					VK_TAB,			SHIFTKEY_THROUGH},
	{Key_Backspace,				VK_BACK,		SHIFTKEY_THROUGH},
	{Key_Return,				VK_RETURN,		SHIFTKEY_THROUGH},
	{Key_Enter,					VK_RETURN,		SHIFTKEY_THROUGH},
	{Key_Insert,				VK_INSERT,		SHIFTKEY_THROUGH},
	{Key_Delete,				VK_DELETE,		SHIFTKEY_THROUGH},
	{Key_Pause,					VK_PAUSE,		SHIFTKEY_THROUGH},
	{Key_Print,					VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Key_SysReq,				VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Key_Clear,					VK_CLEAR,		SHIFTKEY_THROUGH},
	{Key_Home,					VK_HOME,		SHIFTKEY_THROUGH},
	{Key_End,					VK_END,			SHIFTKEY_THROUGH},
	{Key_Left,					VK_LEFT,		SHIFTKEY_THROUGH},
	{Key_Up,					VK_UP,			SHIFTKEY_THROUGH},
	{Key_Right,					VK_RIGHT,		SHIFTKEY_THROUGH},
	{Key_Down,					VK_DOWN,		SHIFTKEY_THROUGH},
	{Key_PageUp,				VK_PRIOR,		SHIFTKEY_THROUGH},
	{Key_PageDown,				VK_NEXT,		SHIFTKEY_THROUGH},
	{Key_Shift,					VK_SHIFT,		SHIFTKEY_THROUGH},
#if defined(Q_OS_OSX)
	{Key_Control,				VK_LWIN,		SHIFTKEY_THROUGH},
	{Key_Meta,					VK_CONTROL,		SHIFTKEY_THROUGH},
#else // defined(Q_OS_OSX)
	{Key_Control,				VK_CONTROL,		SHIFTKEY_THROUGH},
	{Key_Meta,					VK_LWIN,		SHIFTKEY_THROUGH},
#endif // defined(Q_OS_OSX)
	{Key_Alt,					VK_MENU,		SHIFTKEY_THROUGH},
	{Key_CapsLock,				VK_CAPITAL,		SHIFTKEY_THROUGH},
	{Key_NumLock,				VK_NUMLOCK,		SHIFTKEY_THROUGH},
	{Key_ScrollLock,			VK_SCROLL,		SHIFTKEY_THROUGH},

	{Key_F1,					VK_F1,			SHIFTKEY_THROUGH},
	{Key_F2,					VK_F2,			SHIFTKEY_THROUGH},
	{Key_F3,					VK_F3,			SHIFTKEY_THROUGH},
	{Key_F4,					VK_F4,			SHIFTKEY_THROUGH},
	{Key_F5,					VK_F5,			SHIFTKEY_THROUGH},
	{Key_F6,					VK_F6,			SHIFTKEY_THROUGH},
	{Key_F7,					VK_F7,			SHIFTKEY_THROUGH},
	{Key_F8,					VK_F8,			SHIFTKEY_THROUGH},
	{Key_F9,					VK_F9,			SHIFTKEY_THROUGH},
	{Key_F10,					VK_F10,			SHIFTKEY_THROUGH},
	{Key_F11,					VK_F11,			SHIFTKEY_THROUGH},
	{Key_F12,					VK_F12,			SHIFTKEY_THROUGH},
	{Key_F13,					VK_F13,			SHIFTKEY_THROUGH},
	{Key_F14,					VK_F14,			SHIFTKEY_THROUGH},
	{Key_F15,					VK_F15,			SHIFTKEY_THROUGH},
	{Key_F16,					VK_F16,			SHIFTKEY_THROUGH},
	{Key_F17,					VK_F17,			SHIFTKEY_THROUGH},
	{Key_F18,					VK_F18,			SHIFTKEY_THROUGH},
	{Key_F19,					VK_F19,			SHIFTKEY_THROUGH},
	{Key_F20,					VK_F20,			SHIFTKEY_THROUGH},
	{Key_F21,					VK_F21,			SHIFTKEY_THROUGH},
	{Key_F22,					VK_F22,			SHIFTKEY_THROUGH},
	{Key_F23,					VK_F23,			SHIFTKEY_THROUGH},
	{Key_F24,					VK_F24,			SHIFTKEY_THROUGH},

	{Key_0,						VK_0,			SHIFTKEY_NONEED},
	{Key_1,						VK_1,			SHIFTKEY_NONEED},
	{Key_2,						VK_2,			SHIFTKEY_NONEED},
	{Key_3,						VK_3,			SHIFTKEY_NONEED},
	{Key_4,						VK_4,			SHIFTKEY_NONEED},
	{Key_5,						VK_5,			SHIFTKEY_NONEED},
	{Key_6,						VK_6,			SHIFTKEY_NONEED},
	{Key_7,						VK_7,			SHIFTKEY_NONEED},
	{Key_8,						VK_8,			SHIFTKEY_NONEED},
	{Key_9,						VK_9,			SHIFTKEY_NONEED},

	{Key_A,						VK_A,			SHIFTKEY_THROUGH},
	{Key_B,						VK_B,			SHIFTKEY_THROUGH},
	{Key_C,						VK_C,			SHIFTKEY_THROUGH},
	{Key_D,						VK_D,			SHIFTKEY_THROUGH},
	{Key_E,						VK_E,			SHIFTKEY_THROUGH},
	{Key_F,						VK_F,			SHIFTKEY_THROUGH},
	{Key_G,						VK_G,			SHIFTKEY_THROUGH},
	{Key_H,						VK_H,			SHIFTKEY_THROUGH},
	{Key_I,						VK_I,			SHIFTKEY_THROUGH},
	{Key_J,						VK_J,			SHIFTKEY_THROUGH},
	{Key_K,						VK_K,			SHIFTKEY_THROUGH},
	{Key_L,						VK_L,			SHIFTKEY_THROUGH},
	{Key_M,						VK_M,			SHIFTKEY_THROUGH},
	{Key_N,						VK_N,			SHIFTKEY_THROUGH},
	{Key_O,						VK_O,			SHIFTKEY_THROUGH},
	{Key_P,						VK_P,			SHIFTKEY_THROUGH},
	{Key_Q,						VK_Q,			SHIFTKEY_THROUGH},
	{Key_R,						VK_R,			SHIFTKEY_THROUGH},
	{Key_S,						VK_S,			SHIFTKEY_THROUGH},
	{Key_T,						VK_T,			SHIFTKEY_THROUGH},
	{Key_U,						VK_U,			SHIFTKEY_THROUGH},
	{Key_V,						VK_V,			SHIFTKEY_THROUGH},
	{Key_W,						VK_W,			SHIFTKEY_THROUGH},
	{Key_X,						VK_X,			SHIFTKEY_THROUGH},
	{Key_Y,						VK_Y,			SHIFTKEY_THROUGH},
	{Key_Z,						VK_Z,			SHIFTKEY_THROUGH},

	{Key_Space,					VK_SPACE,		SHIFTKEY_THROUGH},

	{Key_Exclam,				VK_1,			SHIFTKEY_NEED},		// '!'
	{Key_QuoteDbl,				VK_OEM_7,		SHIFTKEY_NEED},		// '"'
	{Key_NumberSign,			VK_3,			SHIFTKEY_NEED},		// '#'
	{Key_Dollar,				VK_4,			SHIFTKEY_NEED},		// '$'
	{Key_Percent,				VK_5,			SHIFTKEY_NEED},		// '%'
	{Key_Ampersand,				VK_7,			SHIFTKEY_NEED},		// '&'
	{Key_Apostrophe,			VK_OEM_7,		SHIFTKEY_NONEED},	// '''
	{Key_ParenLeft,				VK_9,			SHIFTKEY_NEED},		// '('
	{Key_ParenRight,			VK_0,			SHIFTKEY_NEED},		// ')'

	{Key_Asterisk,				VK_8,			SHIFTKEY_NEED},		// '*'
	{Key_Plus,					VK_OEM_PLUS,	SHIFTKEY_NEED},		// '+'
	{Key_Comma,					VK_OEM_COMMA,	SHIFTKEY_NONEED},	// ','
	{Key_Minus,					VK_OEM_MINUS,	SHIFTKEY_NONEED},	// '-'
	{Key_Period,				VK_OEM_PERIOD,	SHIFTKEY_NONEED},	// '.'
	{Key_Slash,					VK_OEM_2,		SHIFTKEY_NONEED},	// '/'

	{Key_Colon,					VK_OEM_1,		SHIFTKEY_NEED},		// ':'
	{Key_Semicolon,				VK_OEM_1,		SHIFTKEY_NONEED},	// ';'
	{Key_Less,					VK_OEM_COMMA,	SHIFTKEY_NEED},		// '<'
	{Key_Equal,					VK_OEM_PLUS,	SHIFTKEY_NONEED},	// '='
	{Key_Greater,				VK_OEM_PERIOD,	SHIFTKEY_NEED},		// '>'
	{Key_Question,				VK_OEM_2,		SHIFTKEY_NEED},		// '?'
	{Key_At,					VK_2,			SHIFTKEY_NEED},		// '@'
	{Key_QuoteLeft,				VK_OEM_3,		SHIFTKEY_NONEED},	// '`'

	{Key_BracketLeft,			VK_OEM_4,		SHIFTKEY_NONEED},	// '['
	{Key_BraceLeft,				VK_OEM_4,		SHIFTKEY_NEED},		// '{'
	{Key_Backslash,				VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Key_yen,					VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Key_Bar,					VK_OEM_5,		SHIFTKEY_NEED},		// '|'
	{Key_Underscore,			VK_OEM_MINUS,	SHIFTKEY_NEED},		// '_'
	{Key_BracketRight,			VK_OEM_6,		SHIFTKEY_NONEED},	// ']'
	{Key_BraceRight,			VK_OEM_6,		SHIFTKEY_NEED},		// '}'
	{Key_AsciiTilde,			VK_OEM_3,		SHIFTKEY_NONEED},	// '~'
	{Key_AsciiCircum,			VK_6,			SHIFTKEY_NEED},		// '^'

	{Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// Convert
	{Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// NonConvert
	{Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// Zenkaku_Hankaku

	{Key_Back,					VK_BROWSER_BACK,		SHIFTKEY_THROUGH},
	{Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Key_Refresh,				VK_BROWSER_REFRESH,		SHIFTKEY_THROUGH},
	{Key_Stop,					VK_BROWSER_STOP,		SHIFTKEY_THROUGH},
	{Key_Search,				VK_BROWSER_SEARCH,		SHIFTKEY_THROUGH},
	{Key_Favorites,				VK_BROWSER_FAVORITES,	SHIFTKEY_THROUGH},
	{Key_HomePage,				VK_BROWSER_HOME,		SHIFTKEY_THROUGH},
	{Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Key_VolumeMute,			VK_VOLUME_MUTE,			SHIFTKEY_THROUGH},
	{Key_VolumeDown,			VK_VOLUME_DOWN,			SHIFTKEY_THROUGH},
	{Key_VolumeUp,				VK_VOLUME_UP,			SHIFTKEY_THROUGH},
	{Key_MediaNext,				VK_MEDIA_NEXT_TRACK,	SHIFTKEY_THROUGH},
	{Key_MediaPrevious,			VK_MEDIA_PREV_TRACK,	SHIFTKEY_THROUGH},
	{Key_MediaStop,				VK_MEDIA_STOP,			SHIFTKEY_THROUGH},
	{Key_MediaTogglePlayPause,	VK_MEDIA_PLAY_PAUSE,	SHIFTKEY_THROUGH},
	{Key_LaunchMail,			VK_LAUNCH_MAIL,			SHIFTKEY_THROUGH}
  };

  // key top type
  KEYTOP_TYPE type;

  // shift key control
  ShiftKeyControl shiftKeyControl;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // EVENTCONVERTER_H
