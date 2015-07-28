// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_keybord_jp.h

#ifndef SOFTWARE_KEYBOARD_JP_H
#define SOFTWARE_KEYBOARD_JP_H
// Qt Header

// Local Header
#include "software_keyboard.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

  // VK_Code table
  const static char VK_Code[SoftwareKeyboard::ID_KEY_NUM+1] = {
	VK_NONE_00, // DUMMY
	VK_ESCAPE,VK_1,VK_2,VK_3,VK_4,VK_5,VK_6,VK_7,VK_8,VK_9,VK_0,VK_OEM_MINUS,VK_OEM_7,VK_OEM_5,VK_BACK,
	VK_TAB,VK_Q,VK_W,VK_E,VK_R,VK_T,VK_Y,VK_U,VK_I,VK_O,VK_P,VK_OEM_3,VK_OEM_4,VK_RETURN,
	VK_LCONTROL,VK_A,VK_S,VK_D,VK_F,VK_G,VK_H,VK_J,VK_K,VK_L,VK_OEM_PLUS,VK_OEM_1,VK_OEM_6,
	VK_LSHIFT,VK_Z,VK_X,VK_C,VK_V,VK_B,VK_N,VK_M,VK_OEM_COMMA,VK_OEM_PERIOD,VK_OEM_2,VK_OEM_102,VK_UP,VK_RSHIFT,
	VK_NONE_00,VK_LMENU,VK_LWIN,VK_KANJI,VK_SPACE,VK_RCONTROL,VK_RMENU,VK_NONE_00,VK_LEFT,VK_DOWN,VK_RIGHT
  };

  // VK_Code table with Fn
  const static char VK_CodeWithFn[SoftwareKeyboard::ID_KEY_NUM+1] = {
	VK_NONE_00, // DUMMY
	VK_ESCAPE,VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12,VK_INSERT,VK_DELETE,
	VK_TAB,VK_Q,VK_W,VK_E,VK_R,VK_T,VK_Y,VK_U,VK_I,VK_O,VK_P,VK_NONE_00,VK_NONE_00,VK_RETURN,
	VK_LCONTROL,VK_A,VK_S,VK_D,VK_F,VK_G,VK_H,VK_J,VK_K,VK_L,VK_NONE_00,VK_PAUSE,VK_SNAPSHOT,
	VK_LSHIFT,VK_Z,VK_X,VK_C,VK_V,VK_B,VK_N,VK_M,VK_NONE_00,VK_NONE_00,VK_NONE_00,VK_NONE_00,VK_PRIOR,VK_RSHIFT,
	VK_NONE_00,VK_LMENU,VK_LWIN,VK_KANJI,VK_SPACE,VK_RCONTROL,VK_RMENU,VK_NONE_00,VK_HOME,VK_NEXT,VK_END
  };

  // key top text table with Fn
  const static string keyTopText[SoftwareKeyboard::ID_KEY_NUM+1] = {
	"", // DUMMY
	"ESC","1","2","3","4","5","6","7","8","9","0","-","^","\\","BS",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","@","[","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L",";",":","]",
	"Shift","Z","X","C","V","B","N","M",",",".","/","\\","\342\206\221","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","\342\206\220","\342\206\223","\342\206\222"
  };

  const static string keyTopTextWithShift[SoftwareKeyboard::ID_KEY_NUM+1] = {
	"", // DUMMY
	"ESC","!","\"","#","$","%","&","'","(",")","0","=","~","|","BS",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","`","{","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L","+","*","}",
	"Shift","Z","X","C","V","B","N","M","<",">","?","_","\342\206\221","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","\342\206\220","\342\206\223","\342\206\222"
  };

  const static string keyTopTextWithFn[SoftwareKeyboard::ID_KEY_NUM+1] = {
	"", // DUMMY
	"ESC","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","Ins","Del",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","","","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L","","Pause","PrtS",
	"Shift","Z","X","C","V","B","N","M","","","","","PgU","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","Home","PgD","End"
  };

// SoftwareKeyboard_JP
class SoftwareKeyboard_JP : public SoftwareKeyboard
{
public:
  // constructor
  SoftwareKeyboard_JP(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent = 0);

protected:
  // pressed key
  void pressedKey(ID_KEY id);

  // released key
  void releasedKey(ID_KEY id);

private:
  // initialize key top text
  void initializeKeyTopText();

  // shift key
  void pressedShiftKey();

  // Fn key
  void pressedFnKey();
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_JP_H
