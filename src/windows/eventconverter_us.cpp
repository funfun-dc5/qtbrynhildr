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
  Qt::Key key = (Qt::Key)keyEvent->key();
  bool onKeypadModifier = (keyEvent->modifiers() & Qt::KeypadModifier) == Qt::KeypadModifier;

  shiftKeyControl = SHIFTKEY_THROUGH;

  switch(key){
  case Qt::Key_Escape:
	return VK_ESCAPE;
  case Qt::Key_Tab:
	return VK_TAB;
  case Qt::Key_Backspace:
	return VK_BACK;
  case Qt::Key_Return:
  case Qt::Key_Enter:
	return VK_RETURN;
  case Qt::Key_Insert:
	return VK_INSERT;
  case Qt::Key_Delete:
	return VK_DELETE;
  case Qt::Key_Pause:
	return VK_PAUSE;
  case Qt::Key_Print:
  case Qt::Key_SysReq:
	return VK_SNAPSHOT;
  case Qt::Key_Clear:
	return VK_CLEAR;
  case Qt::Key_Home:
	return VK_HOME;
  case Qt::Key_End:
	return VK_END;
  case Qt::Key_Left:
	return VK_LEFT;
  case Qt::Key_Up:
	return VK_UP;
  case Qt::Key_Right:
	return VK_RIGHT;
  case Qt::Key_Down:
	return VK_DOWN;
  case Qt::Key_PageUp:
	return VK_PRIOR;
  case Qt::Key_PageDown:
	return VK_NEXT;
  case Qt::Key_Shift:
	return VK_SHIFT;
#if defined(Q_OS_OSX)
  case Qt::Key_Control:	// On Mac OS X, Command key
	return VK_LWIN;
  case Qt::Key_Meta:	// On Mac OS X, Control key. On Windows, Windows key.
	return VK_CONTROL;
#else // defined(Q_OS_OSX)
  case Qt::Key_Control:	// On Mac OS X, Command key
	return VK_CONTROL;
  case Qt::Key_Meta:	// On Mac OS X, Control key. On Windows, Windows key.
	return VK_LWIN;
#endif // defined(Q_OS_OSX)
  case Qt::Key_Alt:
	return VK_MENU;
  case Qt::Key_CapsLock:
	return VK_CAPITAL;
  case Qt::Key_NumLock:
	return VK_NUMLOCK;
  case Qt::Key_ScrollLock:
	return VK_SCROLL;

  case Qt::Key_F1:
	return VK_F1;
  case Qt::Key_F2:
	return VK_F2;
  case Qt::Key_F3:
	return VK_F3;
  case Qt::Key_F4:
	return VK_F4;
  case Qt::Key_F5:
	return VK_F5;
  case Qt::Key_F6:
	return VK_F6;
  case Qt::Key_F7:
	return VK_F7;
  case Qt::Key_F8:
	return VK_F8;
  case Qt::Key_F9:
	return VK_F9;
  case Qt::Key_F10:
	return VK_F10;
  case Qt::Key_F11:
	return VK_F11;
  case Qt::Key_F12:
	return VK_F12;
  case Qt::Key_F13:
	return VK_F13;
  case Qt::Key_F14:
	return VK_F14;
  case Qt::Key_F15:
	return VK_F15;
  case Qt::Key_F16:
	return VK_F16;
  case Qt::Key_F17:
	return VK_F17;
  case Qt::Key_F18:
	return VK_F18;
  case Qt::Key_F19:
	return VK_F19;
  case Qt::Key_F20:
	return VK_F20;
  case Qt::Key_F21:
	return VK_F21;
  case Qt::Key_F22:
	return VK_F22;
  case Qt::Key_F23:
	return VK_F23;
  case Qt::Key_F24:
	return VK_F24;

  case Qt::Key_0:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_0;
  case Qt::Key_1:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_1;
  case Qt::Key_2:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_2;
  case Qt::Key_3:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_3;
  case Qt::Key_4:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_4;
  case Qt::Key_5:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_5;
  case Qt::Key_6:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_6;
  case Qt::Key_7:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_7;
  case Qt::Key_8:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_8;
  case Qt::Key_9:
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_9;

  case Qt::Key_A:
	return VK_A;
  case Qt::Key_B:
	return VK_B;
  case Qt::Key_C:
	return VK_C;
  case Qt::Key_D:
	return VK_D;
  case Qt::Key_E:
	return VK_E;
  case Qt::Key_F:
	return VK_F;
  case Qt::Key_G:
	return VK_G;
  case Qt::Key_H:
	return VK_H;
  case Qt::Key_I:
	return VK_I;
  case Qt::Key_J:
	return VK_J;
  case Qt::Key_K:
	return VK_K;
  case Qt::Key_L:
	return VK_L;
  case Qt::Key_M:
	return VK_M;
  case Qt::Key_N:
	return VK_N;
  case Qt::Key_O:
	return VK_O;
  case Qt::Key_P:
	return VK_P;
  case Qt::Key_Q:
	return VK_Q;
  case Qt::Key_R:
	return VK_R;
  case Qt::Key_S:
	return VK_S;
  case Qt::Key_T:
	return VK_T;
  case Qt::Key_U:
	return VK_U;
  case Qt::Key_V:
	return VK_V;
  case Qt::Key_W:
	return VK_W;
  case Qt::Key_X:
	return VK_X;
  case Qt::Key_Y:
	return VK_Y;
  case Qt::Key_Z:
	return VK_Z;

  case Qt::Key_Space:
	return VK_SPACE;

#if 0 // for TEST
  case Qt::Key_Menu:
	return VK_NONE_00;
#endif

  //------------------------------------------------------------
  //
  //------------------------------------------------------------
  case Qt::Key_Exclam:			// '!'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_1;
  case Qt::Key_QuoteDbl:		// '"'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_7;
  case Qt::Key_NumberSign:		// '#'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_3;
  case Qt::Key_Dollar:			// '$'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_4;
  case Qt::Key_Percent:			// '%'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_5;
  case Qt::Key_Ampersand:		// '&'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_7;
  case Qt::Key_Apostrophe:		// '''
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_7;
  case Qt::Key_ParenLeft:		// '('
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_9;
  case Qt::Key_ParenRight:		// ')'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_0;
  case Qt::Key_Asterisk:		// '*'
	if (onKeypadModifier){
	  return VK_MULTIPLY;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NEED;
	  return VK_8;
	}
  case Qt::Key_Plus:			// '+'
	if (onKeypadModifier){
	  return VK_ADD;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NEED;
	  return VK_OEM_PLUS;
	}
  case Qt::Key_Comma:			// ','
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_COMMA;
  case Qt::Key_Minus:			// '-'
	if (onKeypadModifier){
	  return VK_SUBTRACT;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_MINUS;
	}
  case Qt::Key_Period:			// '.'
	if (onKeypadModifier){
	  return VK_DECIMAL;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_PERIOD;
	}
  case Qt::Key_Slash:			// '/'
	if (onKeypadModifier){
	  return VK_DIVIDE;
	}
	else {
	  shiftKeyControl = SHIFTKEY_NONEED;
	  return VK_OEM_2;
	}

  case Qt::Key_Colon:			// ':'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_1;
  case Qt::Key_Semicolon:		// ';'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_1;
  case Qt::Key_Less:			// '<'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_COMMA;
  case Qt::Key_Equal:			// '='
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_PLUS;
  case Qt::Key_Greater:			// '>'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_PERIOD;
  case Qt::Key_Question:		// '?'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_2;
  case Qt::Key_At:				// '@'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_2;
  case Qt::Key_QuoteLeft:		// '`'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_3;

  case Qt::Key_BracketLeft:		// '['
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_4;
  case Qt::Key_BraceLeft:		// '{'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_4;
  case Qt::Key_Backslash:		// '\'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_5;
  case Qt::Key_Bar:				// '|'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_5;
  case Qt::Key_Underscore:		// '_'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_MINUS;
  case Qt::Key_BracketRight:	// ']'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_6;
  case Qt::Key_BraceRight:		// '}'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_OEM_6;
  case Qt::Key_AsciiTilde:		// '~'
	shiftKeyControl = SHIFTKEY_NONEED;
	return VK_OEM_3;
  case Qt::Key_AsciiCircum:		// '^'
	shiftKeyControl = SHIFTKEY_NEED;
	return VK_6;

  //------------------------------------------------------------
  // Extra Keys
  //------------------------------------------------------------
  case Qt::Key_Back:
	return VK_BROWSER_BACK;
  case Qt::Key_Forward:
	return VK_BROWSER_FORWARD;
  case Qt::Key_Refresh:
	return VK_BROWSER_REFRESH;
  case Qt::Key_Stop:
	return VK_BROWSER_STOP;
  case Qt::Key_Search:
	return VK_BROWSER_SEARCH;
  case Qt::Key_Favorites:
	return VK_BROWSER_FAVORITES;
  case Qt::Key_HomePage:
	return VK_BROWSER_HOME;
  case Qt::Key_VolumeMute:
	return VK_VOLUME_MUTE;
  case Qt::Key_VolumeDown:
	return VK_VOLUME_DOWN;
  case Qt::Key_VolumeUp:
	return VK_VOLUME_UP;
  case Qt::Key_MediaNext:
	return VK_MEDIA_NEXT_TRACK;
  case Qt::Key_MediaPrevious:
	return VK_MEDIA_PREV_TRACK;
  case Qt::Key_MediaStop:
	return VK_MEDIA_STOP;
  case Qt::Key_MediaTogglePlayPause:
	return VK_MEDIA_PLAY_PAUSE;
  case Qt::Key_LaunchMail:
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
