// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt Header
#include <QString>

// Local Header
#include "eventconverter_us.h"

using namespace std;

namespace qtbrynhildr {

// get VK code for windows from Qt::Key
// for US Keyboard
uchar EventConverter_US::getVKCode(QKeyEvent *keyEvent)
{
  Key key = (Key)keyEvent->key();
  bool onKeypadModifier = (keyEvent->modifiers() & KeypadModifier) == KeypadModifier;

  shiftKeyControl = SHIFTKEY_THROUGH;

  switch(key){
  case Key_Escape:
	return VK_ESCAPE;
  case Key_Tab:
	return VK_TAB;
  case Key_Backspace:
	return VK_BACK;
  case Key_Return:
  case Key_Enter:
	return VK_RETURN;
  case Key_Insert:
	return VK_INSERT;
  case Key_Delete:
	return VK_DELETE;
  case Key_Pause:
	return VK_PAUSE;
  case Key_Print:
  case Key_SysReq:
	return VK_SNAPSHOT;
  case Key_Clear:
	return VK_CLEAR;
  case Key_Home:
	return VK_HOME;
  case Key_End:
	return VK_END;
  case Key_Left:
	return VK_LEFT;
  case Key_Up:
	return VK_UP;
  case Key_Right:
	return VK_RIGHT;
  case Key_Down:
	return VK_DOWN;
  case Key_PageUp:
	return VK_PRIOR;
  case Key_PageDown:
	return VK_NEXT;
  case Key_Shift:
	return VK_SHIFT;
#if defined(Q_OS_OSX)
  case Key_Control:	// On Mac OS X, Command key
	return VK_LWIN;
  case Key_Meta:	// On Mac OS X, Control key. On Windows, Windows key.
	return VK_CONTROL;
#else // defined(Q_OS_OSX)
  case Key_Control:	// On Mac OS X, Command key
	return VK_CONTROL;
  case Key_Meta:	// On Mac OS X, Control key. On Windows, Windows key.
	return VK_LWIN;
#endif // defined(Q_OS_OSX)
  case Key_Alt:
	return VK_MENU;
  case Key_CapsLock:
	return VK_CAPITAL;
  case Key_NumLock:
	return VK_NUMLOCK;
  case Key_ScrollLock:
	return VK_SCROLL;

  case Key_F1:
	return VK_F1;
  case Key_F2:
	return VK_F2;
  case Key_F3:
	return VK_F3;
  case Key_F4:
	return VK_F4;
  case Key_F5:
	return VK_F5;
  case Key_F6:
	return VK_F6;
  case Key_F7:
	return VK_F7;
  case Key_F8:
	return VK_F8;
  case Key_F9:
	return VK_F9;
  case Key_F10:
	return VK_F10;
  case Key_F11:
	return VK_F11;
  case Key_F12:
	return VK_F12;
  case Key_F13:
	return VK_F13;
  case Key_F14:
	return VK_F14;
  case Key_F15:
	return VK_F15;
  case Key_F16:
	return VK_F16;
  case Key_F17:
	return VK_F17;
  case Key_F18:
	return VK_F18;
  case Key_F19:
	return VK_F19;
  case Key_F20:
	return VK_F20;
  case Key_F21:
	return VK_F21;
  case Key_F22:
	return VK_F22;
  case Key_F23:
	return VK_F23;
  case Key_F24:
	return VK_F24;

  case Key_0:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_0;
  case Key_1:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_1;
  case Key_2:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_2;
  case Key_3:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_3;
  case Key_4:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_4;
  case Key_5:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_5;
  case Key_6:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_6;
  case Key_7:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_7;
  case Key_8:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_8;
  case Key_9:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_9;

  case Key_A:
	return VK_A;
  case Key_B:
	return VK_B;
  case Key_C:
	return VK_C;
  case Key_D:
	return VK_D;
  case Key_E:
	return VK_E;
  case Key_F:
	return VK_F;
  case Key_G:
	return VK_G;
  case Key_H:
	return VK_H;
  case Key_I:
	return VK_I;
  case Key_J:
	return VK_J;
  case Key_K:
	return VK_K;
  case Key_L:
	return VK_L;
  case Key_M:
	return VK_M;
  case Key_N:
	return VK_N;
  case Key_O:
	return VK_O;
  case Key_P:
	return VK_P;
  case Key_Q:
	return VK_Q;
  case Key_R:
	return VK_R;
  case Key_S:
	return VK_S;
  case Key_T:
	return VK_T;
  case Key_U:
	return VK_U;
  case Key_V:
	return VK_V;
  case Key_W:
	return VK_W;
  case Key_X:
	return VK_X;
  case Key_Y:
	return VK_Y;
  case Key_Z:
	return VK_Z;

  case Key_Space:
	return VK_SPACE;

#if 0 // for TEST
  case Key_Menu:
	return VK_NONE_00;
#endif

  //------------------------------------------------------------
  //
  //------------------------------------------------------------
  case Key_Exclam:			// '!'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_1;
  case Key_QuoteDbl:		// '"'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_7;
  case Key_NumberSign:		// '#'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_3;
  case Key_Dollar:			// '$'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_4;
  case Key_Percent:			// '%'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_5;
  case Key_Ampersand:		// '&'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_7;
  case Key_Apostrophe:		// '''
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_7;
  case Key_ParenLeft:		// '('
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_9;
  case Key_ParenRight:		// ')'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_0;
  case Key_Asterisk:		// '*'
	if (onKeypadModifier){
	  return VK_MULTIPLY;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NEED;
	  return VK_8;
	}
  case Key_Plus:			// '+'
	if (onKeypadModifier){
	  return VK_ADD;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NEED;
	  return VK_OEM_PLUS;
	}
  case Key_Comma:			// ','
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_COMMA;
  case Key_Minus:			// '-'
	if (onKeypadModifier){
	  return VK_SUBTRACT;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_MINUS;
	}
  case Key_Period:			// '.'
	if (onKeypadModifier){
	  return VK_DECIMAL;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_PERIOD;
	}
  case Key_Slash:			// '/'
	if (onKeypadModifier){
	  return VK_DIVIDE;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_2;
	}

  case Key_Colon:			// ':'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_1;
  case Key_Semicolon:		// ';'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_1;
  case Key_Less:			// '<'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_COMMA;
  case Key_Equal:			// '='
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_PLUS;
  case Key_Greater:			// '>'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_PERIOD;
  case Key_Question:		// '?'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_2;
  case Key_At:				// '@'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_2;
  case Key_QuoteLeft:		// '`'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_3;

  case Key_BracketLeft:		// '['
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_4;
  case Key_BraceLeft:		// '{'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_4;
  case Key_Backslash:		// '\'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_5;
  case Key_Bar:				// '|'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_5;
  case Key_Underscore:		// '_'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_MINUS;
  case Key_BracketRight:	// ']'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_6;
  case Key_BraceRight:		// '}'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_6;
  case Key_AsciiTilde:		// '~'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_3;
  case Key_AsciiCircum:		// '^'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_6;

  //------------------------------------------------------------
  // Extra Keys
  //------------------------------------------------------------
  case Key_Back:
	return VK_BROWSER_BACK;
  case Key_Forward:
	return VK_BROWSER_FORWARD;
  case Key_Refresh:
	return VK_BROWSER_REFRESH;
  case Key_Stop:
	return VK_BROWSER_STOP;
  case Key_Search:
	return VK_BROWSER_SEARCH;
  case Key_Favorites:
	return VK_BROWSER_FAVORITES;
  case Key_HomePage:
	return VK_BROWSER_HOME;
  case Key_VolumeMute:
	return VK_VOLUME_MUTE;
  case Key_VolumeDown:
	return VK_VOLUME_DOWN;
  case Key_VolumeUp:
	return VK_VOLUME_UP;
  case Key_MediaNext:
	return VK_MEDIA_NEXT_TRACK;
  case Key_MediaPrevious:
	return VK_MEDIA_PREV_TRACK;
  case Key_MediaStop:
	return VK_MEDIA_STOP;
  case Key_MediaTogglePlayPause:
	return VK_MEDIA_PLAY_PAUSE;
  case Key_LaunchMail:
	return VK_LAUNCH_MAIL;

  default:					// NOT support key
	if (outputLog){
	  ios::fmtflags flags = cout.flags();
	  cout << "[EventConverter_US] Unknown Key: " << hex << key << endl << flush;
	  cout.flags(flags);
	}
	return VK_NONE_00;
  }
}

// get name
QString EventConverter_US::getEventConverterName()
{
	return (QString)"US";
}

} // end of namespace qtbrynhildr
