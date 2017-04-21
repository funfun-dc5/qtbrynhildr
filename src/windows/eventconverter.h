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

//using namespace Qt;

namespace qtbrynhildr {

class KeyLayoutFile;

// EventConverter
class EventConverter
{
  friend class KeyLayoutFile;

public:
  // keyboard type
  typedef enum {
	KEYTOP_TYPE_JP,
	KEYTOP_TYPE_US,
	KEYTOP_TYPE_NATIVE,
	KEYTOP_TYPE_KLF
  } KEYTOP_TYPE;

#include "windows/keyevent.h"

public:
  // constructor
  EventConverter();
  EventConverter(KEYTOP_TYPE type);
  EventConverter(KeyLayoutFile *klf);

  // get keytop type
  KEYTOP_TYPE getKeytopType();

  // set keytop type
  void setKeytopType(KEYTOP_TYPE type);

  // set keytop type by key layout file
  void setKeytopType(KeyLayoutFile *klf);

  // get Virtual Keycode
  uchar getVKCode(QKeyEvent *keyEvent);

  // get shift key control
  ShiftKeyControl getShiftKeyControl();

  // get name
  QString getEventConverterName() const;

  // get name of virtual keycode
  static QString getVKCodeByString(uchar vkcode);

private:
#if QTB_DEBUG
  // print KeyEvent
  static void printKeyEvent(KeyEvent *keyEvent);
#endif // QTB_DEBUG

private:
  // key table size
#if _MSC_VER
#define TABLE_SIZE_JP 139
#define TABLE_SIZE_US 136
#else // _MSC_VER
  static const int TABLE_SIZE_JP = 139;
  static const int TABLE_SIZE_US = 136;
#endif // _MSC_VER

  // key event table
  const KeyEvent *keyEventTable;

  // table size
  int tableSize;

  // key event table for JP
  const KeyEvent keyEventTable_JP[TABLE_SIZE_JP] = {
	{Qt::Key_Escape,				VK_ESCAPE,		SHIFTKEY_THROUGH},
	{Qt::Key_Tab,					VK_TAB,			SHIFTKEY_THROUGH},
	{Qt::Key_Backspace,				VK_BACK,		SHIFTKEY_THROUGH},
	{Qt::Key_Return,				VK_RETURN,		SHIFTKEY_THROUGH},
	{Qt::Key_Enter,					VK_RETURN,		SHIFTKEY_THROUGH},
	{Qt::Key_Insert,				VK_INSERT,		SHIFTKEY_THROUGH},
	{Qt::Key_Delete,				VK_DELETE,		SHIFTKEY_THROUGH},
	{Qt::Key_Pause,					VK_PAUSE,		SHIFTKEY_THROUGH},
	{Qt::Key_Print,					VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Qt::Key_SysReq,				VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Qt::Key_Clear,					VK_CLEAR,		SHIFTKEY_THROUGH},
	{Qt::Key_Home,					VK_HOME,		SHIFTKEY_THROUGH},
	{Qt::Key_End,					VK_END,			SHIFTKEY_THROUGH},
	{Qt::Key_Left,					VK_LEFT,		SHIFTKEY_THROUGH},
	{Qt::Key_Up,					VK_UP,			SHIFTKEY_THROUGH},
	{Qt::Key_Right,					VK_RIGHT,		SHIFTKEY_THROUGH},
	{Qt::Key_Down,					VK_DOWN,		SHIFTKEY_THROUGH},
	{Qt::Key_PageUp,				VK_PRIOR,		SHIFTKEY_THROUGH},
	{Qt::Key_PageDown,				VK_NEXT,		SHIFTKEY_THROUGH},
	{Qt::Key_Shift,					VK_SHIFT,		SHIFTKEY_THROUGH},
#if defined(Q_OS_OSX)
	{Qt::Key_Control,				VK_LWIN,		SHIFTKEY_THROUGH},
	{Qt::Key_Meta,					VK_CONTROL,		SHIFTKEY_THROUGH},
#else // defined(Q_OS_OSX)
	{Qt::Key_Control,				VK_CONTROL,		SHIFTKEY_THROUGH},
	{Qt::Key_Meta,					VK_LWIN,		SHIFTKEY_THROUGH},
#endif // defined(Q_OS_OSX)
	{Qt::Key_Alt,					VK_MENU,		SHIFTKEY_THROUGH},
	{Qt::Key_CapsLock,				VK_CAPITAL,		SHIFTKEY_THROUGH},
	{Qt::Key_NumLock,				VK_NUMLOCK,		SHIFTKEY_THROUGH},
	{Qt::Key_ScrollLock,			VK_SCROLL,		SHIFTKEY_THROUGH},

	{Qt::Key_F1,					VK_F1,			SHIFTKEY_THROUGH},
	{Qt::Key_F2,					VK_F2,			SHIFTKEY_THROUGH},
	{Qt::Key_F3,					VK_F3,			SHIFTKEY_THROUGH},
	{Qt::Key_F4,					VK_F4,			SHIFTKEY_THROUGH},
	{Qt::Key_F5,					VK_F5,			SHIFTKEY_THROUGH},
	{Qt::Key_F6,					VK_F6,			SHIFTKEY_THROUGH},
	{Qt::Key_F7,					VK_F7,			SHIFTKEY_THROUGH},
	{Qt::Key_F8,					VK_F8,			SHIFTKEY_THROUGH},
	{Qt::Key_F9,					VK_F9,			SHIFTKEY_THROUGH},
	{Qt::Key_F10,					VK_F10,			SHIFTKEY_THROUGH},
	{Qt::Key_F11,					VK_F11,			SHIFTKEY_THROUGH},
	{Qt::Key_F12,					VK_F12,			SHIFTKEY_THROUGH},
	{Qt::Key_F13,					VK_F13,			SHIFTKEY_THROUGH},
	{Qt::Key_F14,					VK_F14,			SHIFTKEY_THROUGH},
	{Qt::Key_F15,					VK_F15,			SHIFTKEY_THROUGH},
	{Qt::Key_F16,					VK_F16,			SHIFTKEY_THROUGH},
	{Qt::Key_F17,					VK_F17,			SHIFTKEY_THROUGH},
	{Qt::Key_F18,					VK_F18,			SHIFTKEY_THROUGH},
	{Qt::Key_F19,					VK_F19,			SHIFTKEY_THROUGH},
	{Qt::Key_F20,					VK_F20,			SHIFTKEY_THROUGH},
	{Qt::Key_F21,					VK_F21,			SHIFTKEY_THROUGH},
	{Qt::Key_F22,					VK_F22,			SHIFTKEY_THROUGH},
	{Qt::Key_F23,					VK_F23,			SHIFTKEY_THROUGH},
	{Qt::Key_F24,					VK_F24,			SHIFTKEY_THROUGH},

	{Qt::Key_0,						VK_0,			SHIFTKEY_NONEED},
	{Qt::Key_1,						VK_1,			SHIFTKEY_NONEED},
	{Qt::Key_2,						VK_2,			SHIFTKEY_NONEED},
	{Qt::Key_3,						VK_3,			SHIFTKEY_NONEED},
	{Qt::Key_4,						VK_4,			SHIFTKEY_NONEED},
	{Qt::Key_5,						VK_5,			SHIFTKEY_NONEED},
	{Qt::Key_6,						VK_6,			SHIFTKEY_NONEED},
	{Qt::Key_7,						VK_7,			SHIFTKEY_NONEED},
	{Qt::Key_8,						VK_8,			SHIFTKEY_NONEED},
	{Qt::Key_9,						VK_9,			SHIFTKEY_NONEED},

	{Qt::Key_A,						VK_A,			SHIFTKEY_THROUGH},
	{Qt::Key_B,						VK_B,			SHIFTKEY_THROUGH},
	{Qt::Key_C,						VK_C,			SHIFTKEY_THROUGH},
	{Qt::Key_D,						VK_D,			SHIFTKEY_THROUGH},
	{Qt::Key_E,						VK_E,			SHIFTKEY_THROUGH},
	{Qt::Key_F,						VK_F,			SHIFTKEY_THROUGH},
	{Qt::Key_G,						VK_G,			SHIFTKEY_THROUGH},
	{Qt::Key_H,						VK_H,			SHIFTKEY_THROUGH},
	{Qt::Key_I,						VK_I,			SHIFTKEY_THROUGH},
	{Qt::Key_J,						VK_J,			SHIFTKEY_THROUGH},
	{Qt::Key_K,						VK_K,			SHIFTKEY_THROUGH},
	{Qt::Key_L,						VK_L,			SHIFTKEY_THROUGH},
	{Qt::Key_M,						VK_M,			SHIFTKEY_THROUGH},
	{Qt::Key_N,						VK_N,			SHIFTKEY_THROUGH},
	{Qt::Key_O,						VK_O,			SHIFTKEY_THROUGH},
	{Qt::Key_P,						VK_P,			SHIFTKEY_THROUGH},
	{Qt::Key_Q,						VK_Q,			SHIFTKEY_THROUGH},
	{Qt::Key_R,						VK_R,			SHIFTKEY_THROUGH},
	{Qt::Key_S,						VK_S,			SHIFTKEY_THROUGH},
	{Qt::Key_T,						VK_T,			SHIFTKEY_THROUGH},
	{Qt::Key_U,						VK_U,			SHIFTKEY_THROUGH},
	{Qt::Key_V,						VK_V,			SHIFTKEY_THROUGH},
	{Qt::Key_W,						VK_W,			SHIFTKEY_THROUGH},
	{Qt::Key_X,						VK_X,			SHIFTKEY_THROUGH},
	{Qt::Key_Y,						VK_Y,			SHIFTKEY_THROUGH},
	{Qt::Key_Z,						VK_Z,			SHIFTKEY_THROUGH},

	{Qt::Key_Space,					VK_SPACE,		SHIFTKEY_THROUGH},

	{Qt::Key_Exclam,				VK_1,			SHIFTKEY_NEED},		// '!'
	{Qt::Key_QuoteDbl,				VK_2,			SHIFTKEY_NEED},		// '"'
	{Qt::Key_NumberSign,			VK_3,			SHIFTKEY_NEED},		// '#'
	{Qt::Key_Dollar,				VK_4,			SHIFTKEY_NEED},		// '$'
	{Qt::Key_Percent,				VK_5,			SHIFTKEY_NEED},		// '%'
	{Qt::Key_Ampersand,				VK_6,			SHIFTKEY_NEED},		// '&'
	{Qt::Key_Apostrophe,			VK_7,			SHIFTKEY_NEED},		// '''
	{Qt::Key_ParenLeft,				VK_8,			SHIFTKEY_NEED},		// '('
	{Qt::Key_ParenRight,			VK_9,			SHIFTKEY_NEED},		// ')'

	{Qt::Key_Asterisk,				VK_OEM_1,		SHIFTKEY_NEED},		// '*'
	{Qt::Key_Plus,					VK_OEM_PLUS,	SHIFTKEY_NEED},		// '+'
	{Qt::Key_Comma,					VK_OEM_COMMA,	SHIFTKEY_NONEED},	// ','
	{Qt::Key_Minus,					VK_OEM_MINUS,	SHIFTKEY_NONEED},	// '-'
	{Qt::Key_Period,				VK_OEM_PERIOD,	SHIFTKEY_NONEED},	// '.'
	{Qt::Key_Slash,					VK_OEM_2,		SHIFTKEY_NONEED},	// '/'

	{Qt::Key_Colon,					VK_OEM_1,		SHIFTKEY_NONEED},	// ':'
	{Qt::Key_Semicolon,				VK_OEM_PLUS,	SHIFTKEY_NONEED},	// ';'
	{Qt::Key_Less,					VK_OEM_COMMA,	SHIFTKEY_NEED},		// '<'
	{Qt::Key_Equal,					VK_OEM_MINUS,	SHIFTKEY_NEED},		// '='
	{Qt::Key_Greater,				VK_OEM_PERIOD,	SHIFTKEY_NEED},		// '>'
	{Qt::Key_Question,				VK_OEM_2,		SHIFTKEY_NEED},		// '?'
	{Qt::Key_At,					VK_OEM_3,		SHIFTKEY_NONEED},	// '@'
	{Qt::Key_QuoteLeft,				VK_OEM_3,		SHIFTKEY_NEED},		// '`'

	{Qt::Key_BracketLeft,			VK_OEM_4,		SHIFTKEY_NONEED},	// '['
	{Qt::Key_BraceLeft,				VK_OEM_4,		SHIFTKEY_NEED},		// '{'
	{Qt::Key_Backslash,				VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Qt::Key_yen,					VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Qt::Key_Bar,					VK_OEM_5,		SHIFTKEY_NEED},		// '|'
	{Qt::Key_Underscore,			VK_OEM_102,		SHIFTKEY_NEED},		// '_'
	{Qt::Key_BracketRight,			VK_OEM_6,		SHIFTKEY_NONEED},	// ']'
	{Qt::Key_BraceRight,			VK_OEM_6,		SHIFTKEY_NEED},		// '}'
	{Qt::Key_AsciiTilde,			VK_OEM_7,		SHIFTKEY_NEED},		// '~'
	{Qt::Key_AsciiCircum,			VK_OEM_7,		SHIFTKEY_NONEED},	// '^'

	{Qt::Key_Henkan,				VK_CONVERT,		SHIFTKEY_THROUGH},	// Convert
	{Qt::Key_Muhenkan,				VK_NONCONVERT,	SHIFTKEY_THROUGH},	// NonConvert
	{Qt::Key_Zenkaku_Hankaku,		VK_KANJI,		SHIFTKEY_THROUGH},	// Zenkaku_Hankaku

	{Qt::Key_Back,					VK_BROWSER_BACK,		SHIFTKEY_THROUGH},
	{Qt::Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Qt::Key_Refresh,				VK_BROWSER_REFRESH,		SHIFTKEY_THROUGH},
	{Qt::Key_Stop,					VK_BROWSER_STOP,		SHIFTKEY_THROUGH},
	{Qt::Key_Search,				VK_BROWSER_SEARCH,		SHIFTKEY_THROUGH},
	{Qt::Key_Favorites,				VK_BROWSER_FAVORITES,	SHIFTKEY_THROUGH},
	{Qt::Key_HomePage,				VK_BROWSER_HOME,		SHIFTKEY_THROUGH},
	{Qt::Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Qt::Key_VolumeMute,			VK_VOLUME_MUTE,			SHIFTKEY_THROUGH},
	{Qt::Key_VolumeDown,			VK_VOLUME_DOWN,			SHIFTKEY_THROUGH},
	{Qt::Key_VolumeUp,				VK_VOLUME_UP,			SHIFTKEY_THROUGH},
	{Qt::Key_MediaNext,				VK_MEDIA_NEXT_TRACK,	SHIFTKEY_THROUGH},
	{Qt::Key_MediaPrevious,			VK_MEDIA_PREV_TRACK,	SHIFTKEY_THROUGH},
	{Qt::Key_MediaStop,				VK_MEDIA_STOP,			SHIFTKEY_THROUGH},
	{Qt::Key_MediaTogglePlayPause,	VK_MEDIA_PLAY_PAUSE,	SHIFTKEY_THROUGH},
	{Qt::Key_LaunchMail,			VK_LAUNCH_MAIL,			SHIFTKEY_THROUGH}
  };

  // key event table for US
  const KeyEvent keyEventTable_US[TABLE_SIZE_US] = {
	{Qt::Key_Escape,				VK_ESCAPE,		SHIFTKEY_THROUGH},
	{Qt::Key_Tab,					VK_TAB,			SHIFTKEY_THROUGH},
	{Qt::Key_Backspace,				VK_BACK,		SHIFTKEY_THROUGH},
	{Qt::Key_Return,				VK_RETURN,		SHIFTKEY_THROUGH},
	{Qt::Key_Enter,					VK_RETURN,		SHIFTKEY_THROUGH},
	{Qt::Key_Insert,				VK_INSERT,		SHIFTKEY_THROUGH},
	{Qt::Key_Delete,				VK_DELETE,		SHIFTKEY_THROUGH},
	{Qt::Key_Pause,					VK_PAUSE,		SHIFTKEY_THROUGH},
	{Qt::Key_Print,					VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Qt::Key_SysReq,				VK_SNAPSHOT,	SHIFTKEY_THROUGH},
	{Qt::Key_Clear,					VK_CLEAR,		SHIFTKEY_THROUGH},
	{Qt::Key_Home,					VK_HOME,		SHIFTKEY_THROUGH},
	{Qt::Key_End,					VK_END,			SHIFTKEY_THROUGH},
	{Qt::Key_Left,					VK_LEFT,		SHIFTKEY_THROUGH},
	{Qt::Key_Up,					VK_UP,			SHIFTKEY_THROUGH},
	{Qt::Key_Right,					VK_RIGHT,		SHIFTKEY_THROUGH},
	{Qt::Key_Down,					VK_DOWN,		SHIFTKEY_THROUGH},
	{Qt::Key_PageUp,				VK_PRIOR,		SHIFTKEY_THROUGH},
	{Qt::Key_PageDown,				VK_NEXT,		SHIFTKEY_THROUGH},
	{Qt::Key_Shift,					VK_SHIFT,		SHIFTKEY_THROUGH},
#if defined(Q_OS_OSX)
	{Qt::Key_Control,				VK_LWIN,		SHIFTKEY_THROUGH},
	{Qt::Key_Meta,					VK_CONTROL,		SHIFTKEY_THROUGH},
#else // defined(Q_OS_OSX)
	{Qt::Key_Control,				VK_CONTROL,		SHIFTKEY_THROUGH},
	{Qt::Key_Meta,					VK_LWIN,		SHIFTKEY_THROUGH},
#endif // defined(Q_OS_OSX)
	{Qt::Key_Alt,					VK_MENU,		SHIFTKEY_THROUGH},
	{Qt::Key_CapsLock,				VK_CAPITAL,		SHIFTKEY_THROUGH},
	{Qt::Key_NumLock,				VK_NUMLOCK,		SHIFTKEY_THROUGH},
	{Qt::Key_ScrollLock,			VK_SCROLL,		SHIFTKEY_THROUGH},

	{Qt::Key_F1,					VK_F1,			SHIFTKEY_THROUGH},
	{Qt::Key_F2,					VK_F2,			SHIFTKEY_THROUGH},
	{Qt::Key_F3,					VK_F3,			SHIFTKEY_THROUGH},
	{Qt::Key_F4,					VK_F4,			SHIFTKEY_THROUGH},
	{Qt::Key_F5,					VK_F5,			SHIFTKEY_THROUGH},
	{Qt::Key_F6,					VK_F6,			SHIFTKEY_THROUGH},
	{Qt::Key_F7,					VK_F7,			SHIFTKEY_THROUGH},
	{Qt::Key_F8,					VK_F8,			SHIFTKEY_THROUGH},
	{Qt::Key_F9,					VK_F9,			SHIFTKEY_THROUGH},
	{Qt::Key_F10,					VK_F10,			SHIFTKEY_THROUGH},
	{Qt::Key_F11,					VK_F11,			SHIFTKEY_THROUGH},
	{Qt::Key_F12,					VK_F12,			SHIFTKEY_THROUGH},
	{Qt::Key_F13,					VK_F13,			SHIFTKEY_THROUGH},
	{Qt::Key_F14,					VK_F14,			SHIFTKEY_THROUGH},
	{Qt::Key_F15,					VK_F15,			SHIFTKEY_THROUGH},
	{Qt::Key_F16,					VK_F16,			SHIFTKEY_THROUGH},
	{Qt::Key_F17,					VK_F17,			SHIFTKEY_THROUGH},
	{Qt::Key_F18,					VK_F18,			SHIFTKEY_THROUGH},
	{Qt::Key_F19,					VK_F19,			SHIFTKEY_THROUGH},
	{Qt::Key_F20,					VK_F20,			SHIFTKEY_THROUGH},
	{Qt::Key_F21,					VK_F21,			SHIFTKEY_THROUGH},
	{Qt::Key_F22,					VK_F22,			SHIFTKEY_THROUGH},
	{Qt::Key_F23,					VK_F23,			SHIFTKEY_THROUGH},
	{Qt::Key_F24,					VK_F24,			SHIFTKEY_THROUGH},

	{Qt::Key_0,						VK_0,			SHIFTKEY_NONEED},
	{Qt::Key_1,						VK_1,			SHIFTKEY_NONEED},
	{Qt::Key_2,						VK_2,			SHIFTKEY_NONEED},
	{Qt::Key_3,						VK_3,			SHIFTKEY_NONEED},
	{Qt::Key_4,						VK_4,			SHIFTKEY_NONEED},
	{Qt::Key_5,						VK_5,			SHIFTKEY_NONEED},
	{Qt::Key_6,						VK_6,			SHIFTKEY_NONEED},
	{Qt::Key_7,						VK_7,			SHIFTKEY_NONEED},
	{Qt::Key_8,						VK_8,			SHIFTKEY_NONEED},
	{Qt::Key_9,						VK_9,			SHIFTKEY_NONEED},

	{Qt::Key_A,						VK_A,			SHIFTKEY_THROUGH},
	{Qt::Key_B,						VK_B,			SHIFTKEY_THROUGH},
	{Qt::Key_C,						VK_C,			SHIFTKEY_THROUGH},
	{Qt::Key_D,						VK_D,			SHIFTKEY_THROUGH},
	{Qt::Key_E,						VK_E,			SHIFTKEY_THROUGH},
	{Qt::Key_F,						VK_F,			SHIFTKEY_THROUGH},
	{Qt::Key_G,						VK_G,			SHIFTKEY_THROUGH},
	{Qt::Key_H,						VK_H,			SHIFTKEY_THROUGH},
	{Qt::Key_I,						VK_I,			SHIFTKEY_THROUGH},
	{Qt::Key_J,						VK_J,			SHIFTKEY_THROUGH},
	{Qt::Key_K,						VK_K,			SHIFTKEY_THROUGH},
	{Qt::Key_L,						VK_L,			SHIFTKEY_THROUGH},
	{Qt::Key_M,						VK_M,			SHIFTKEY_THROUGH},
	{Qt::Key_N,						VK_N,			SHIFTKEY_THROUGH},
	{Qt::Key_O,						VK_O,			SHIFTKEY_THROUGH},
	{Qt::Key_P,						VK_P,			SHIFTKEY_THROUGH},
	{Qt::Key_Q,						VK_Q,			SHIFTKEY_THROUGH},
	{Qt::Key_R,						VK_R,			SHIFTKEY_THROUGH},
	{Qt::Key_S,						VK_S,			SHIFTKEY_THROUGH},
	{Qt::Key_T,						VK_T,			SHIFTKEY_THROUGH},
	{Qt::Key_U,						VK_U,			SHIFTKEY_THROUGH},
	{Qt::Key_V,						VK_V,			SHIFTKEY_THROUGH},
	{Qt::Key_W,						VK_W,			SHIFTKEY_THROUGH},
	{Qt::Key_X,						VK_X,			SHIFTKEY_THROUGH},
	{Qt::Key_Y,						VK_Y,			SHIFTKEY_THROUGH},
	{Qt::Key_Z,						VK_Z,			SHIFTKEY_THROUGH},

	{Qt::Key_Space,					VK_SPACE,		SHIFTKEY_THROUGH},

	{Qt::Key_Exclam,				VK_1,			SHIFTKEY_NEED},		// '!'
	{Qt::Key_QuoteDbl,				VK_OEM_7,		SHIFTKEY_NEED},		// '"'
	{Qt::Key_NumberSign,			VK_3,			SHIFTKEY_NEED},		// '#'
	{Qt::Key_Dollar,				VK_4,			SHIFTKEY_NEED},		// '$'
	{Qt::Key_Percent,				VK_5,			SHIFTKEY_NEED},		// '%'
	{Qt::Key_Ampersand,				VK_7,			SHIFTKEY_NEED},		// '&'
	{Qt::Key_Apostrophe,			VK_OEM_7,		SHIFTKEY_NONEED},	// '''
	{Qt::Key_ParenLeft,				VK_9,			SHIFTKEY_NEED},		// '('
	{Qt::Key_ParenRight,			VK_0,			SHIFTKEY_NEED},		// ')'

	{Qt::Key_Asterisk,				VK_8,			SHIFTKEY_NEED},		// '*'
	{Qt::Key_Plus,					VK_OEM_PLUS,	SHIFTKEY_NEED},		// '+'
	{Qt::Key_Comma,					VK_OEM_COMMA,	SHIFTKEY_NONEED},	// ','
	{Qt::Key_Minus,					VK_OEM_MINUS,	SHIFTKEY_NONEED},	// '-'
	{Qt::Key_Period,				VK_OEM_PERIOD,	SHIFTKEY_NONEED},	// '.'
	{Qt::Key_Slash,					VK_OEM_2,		SHIFTKEY_NONEED},	// '/'

	{Qt::Key_Colon,					VK_OEM_1,		SHIFTKEY_NEED},		// ':'
	{Qt::Key_Semicolon,				VK_OEM_1,		SHIFTKEY_NONEED},	// ';'
	{Qt::Key_Less,					VK_OEM_COMMA,	SHIFTKEY_NEED},		// '<'
	{Qt::Key_Equal,					VK_OEM_PLUS,	SHIFTKEY_NONEED},	// '='
	{Qt::Key_Greater,				VK_OEM_PERIOD,	SHIFTKEY_NEED},		// '>'
	{Qt::Key_Question,				VK_OEM_2,		SHIFTKEY_NEED},		// '?'
	{Qt::Key_At,					VK_2,			SHIFTKEY_NEED},		// '@'
	{Qt::Key_QuoteLeft,				VK_OEM_3,		SHIFTKEY_NONEED},	// '`'

	{Qt::Key_BracketLeft,			VK_OEM_4,		SHIFTKEY_NONEED},	// '['
	{Qt::Key_BraceLeft,				VK_OEM_4,		SHIFTKEY_NEED},		// '{'
	{Qt::Key_Backslash,				VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Qt::Key_yen,					VK_OEM_5,		SHIFTKEY_NONEED},	// '\'
	{Qt::Key_Bar,					VK_OEM_5,		SHIFTKEY_NEED},		// '|'
	{Qt::Key_Underscore,			VK_OEM_MINUS,	SHIFTKEY_NEED},		// '_'
	{Qt::Key_BracketRight,			VK_OEM_6,		SHIFTKEY_NONEED},	// ']'
	{Qt::Key_BraceRight,			VK_OEM_6,		SHIFTKEY_NEED},		// '}'
	{Qt::Key_AsciiTilde,			VK_OEM_3,		SHIFTKEY_NONEED},	// '~'
	{Qt::Key_AsciiCircum,			VK_6,			SHIFTKEY_NEED},		// '^'
#if 0 // for TEST
	{Qt::Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// Convert
	{Qt::Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// NonConvert
	{Qt::Key_unknown,				VK_NONE_00,		SHIFTKEY_THROUGH},	// Zenkaku_Hankaku
#endif // for TEST
	{Qt::Key_Back,					VK_BROWSER_BACK,		SHIFTKEY_THROUGH},
	{Qt::Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Qt::Key_Refresh,				VK_BROWSER_REFRESH,		SHIFTKEY_THROUGH},
	{Qt::Key_Stop,					VK_BROWSER_STOP,		SHIFTKEY_THROUGH},
	{Qt::Key_Search,				VK_BROWSER_SEARCH,		SHIFTKEY_THROUGH},
	{Qt::Key_Favorites,				VK_BROWSER_FAVORITES,	SHIFTKEY_THROUGH},
	{Qt::Key_HomePage,				VK_BROWSER_HOME,		SHIFTKEY_THROUGH},
	{Qt::Key_Forward,				VK_BROWSER_FORWARD,		SHIFTKEY_THROUGH},
	{Qt::Key_VolumeMute,			VK_VOLUME_MUTE,			SHIFTKEY_THROUGH},
	{Qt::Key_VolumeDown,			VK_VOLUME_DOWN,			SHIFTKEY_THROUGH},
	{Qt::Key_VolumeUp,				VK_VOLUME_UP,			SHIFTKEY_THROUGH},
	{Qt::Key_MediaNext,				VK_MEDIA_NEXT_TRACK,	SHIFTKEY_THROUGH},
	{Qt::Key_MediaPrevious,			VK_MEDIA_PREV_TRACK,	SHIFTKEY_THROUGH},
	{Qt::Key_MediaStop,				VK_MEDIA_STOP,			SHIFTKEY_THROUGH},
	{Qt::Key_MediaTogglePlayPause,	VK_MEDIA_PLAY_PAUSE,	SHIFTKEY_THROUGH},
	{Qt::Key_LaunchMail,			VK_LAUNCH_MAIL,			SHIFTKEY_THROUGH}
  };

  // key layout file
  KeyLayoutFile *klf;

  // key top type
  KEYTOP_TYPE type;

  // shift key control
  ShiftKeyControl shiftKeyControl;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // EVENTCONVERTER_H
