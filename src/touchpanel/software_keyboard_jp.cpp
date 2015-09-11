// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard_jp.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_keyboard_jp.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

  // key information table
  const static SoftwareKeyboard::KeyTopInfo keyTopInfo[SoftwareKeyboard::ID_KEY_NUM+1] = {
	{{"",		"",		VK_NONE_00		},	{"",		VK_NONE_00	}},	// DUMMY

	{{"ESC",	"ESC",	VK_ESCAPE		},	{"ESC",		VK_ESCAPE	}},	// ID_KEY_1
	{{"1",		"!",	VK_1			},	{"F1",		VK_F1		}},	// ID_KEY_2
	{{"2",		"\"",	VK_2			},	{"F2",		VK_F2		}},	// ID_KEY_3
	{{"3",		"#",	VK_3			},	{"F3",		VK_F3		}},	// ID_KEY_4
	{{"4",		"$",	VK_4			},	{"F4",		VK_F4		}},	// ID_KEY_5
	{{"5",		"%",	VK_5			},	{"F5",		VK_F5		}},	// ID_KEY_6
	{{"6",		"&&",	VK_6			},	{"F6",		VK_F6		}},	// ID_KEY_7
	{{"7",		"'",	VK_7			},	{"F7",		VK_F7		}},	// ID_KEY_8
	{{"8",		"(",	VK_8			},	{"F8",		VK_F8		}},	// ID_KEY_9
	{{"9",		")",	VK_9			},	{"F9",		VK_F9		}},	// ID_KEY_10
	{{"0",		"",		VK_0			},	{"F10",		VK_F10		}},	// ID_KEY_11
	{{"-",		"=",	VK_OEM_MINUS	},	{"F11",		VK_F11		}},	// ID_KEY_12
	{{"^",		"~",	VK_OEM_7		},	{"F12",		VK_F12		}},	// ID_KEY_13
	{{"\\",		"|",	VK_OEM_5		},	{"Ins",		VK_INSERT	}},	// ID_KEY_14
	{{"BS",		"BS",	VK_BACK			},	{"Del",		VK_DELETE	}},	// ID_KEY_15

	{{"Tab",	"Tab",	VK_TAB			},	{"Tab",		VK_TAB		}},	// ID_KEY_16
	{{"Q",		"Q",	VK_Q			},	{"Q",		VK_Q		}},	// ID_KEY_17
	{{"W",		"W",	VK_W			},	{"W",		VK_W		}},	// ID_KEY_18
	{{"E",		"E",	VK_E			},	{"E",		VK_E		}},	// ID_KEY_19
	{{"R",		"R",	VK_R			},	{"R",		VK_R		}},	// ID_KEY_20
	{{"T",		"T",	VK_T			},	{"T",		VK_T		}},	// ID_KEY_21
	{{"Y",		"Y",	VK_Y			},	{"Y",		VK_Y		}},	// ID_KEY_22
	{{"U",		"U",	VK_U			},	{"U",		VK_U		}},	// ID_KEY_23
	{{"I",		"I",	VK_I			},	{"I",		VK_I		}},	// ID_KEY_24
	{{"O",		"O",	VK_O			},	{"O",		VK_O		}},	// ID_KEY_25
	{{"P",		"P",	VK_P			},	{"P",		VK_P		}},	// ID_KEY_26
	{{"@",		"`",	VK_OEM_3		},	{"",		VK_NONE_00	}},	// ID_KEY_27
	{{"[",		"{",	VK_OEM_4		},	{"",		VK_NONE_00	}},	// ID_KEY_28
	{{"RET",	"RET",	VK_RETURN		},	{"RET",		VK_RETURN	}},	// ID_KEY_29

	{{"Ctrl",	"Ctrl",	VK_LCONTROL		},	{"Ctrl",	VK_LCONTROL	}},	// ID_KEY_30
	{{"A",		"A",	VK_A			},	{"A",		VK_A		}},	// ID_KEY_31
	{{"S",		"S",	VK_S			},	{"S",		VK_S		}},	// ID_KEY_32
	{{"D",		"D",	VK_D			},	{"D",		VK_D		}},	// ID_KEY_33
	{{"F",		"F",	VK_F			},	{"F",		VK_F		}},	// ID_KEY_34
	{{"G",		"G",	VK_G			},	{"G",		VK_G		}},	// ID_KEY_35
	{{"H",		"H",	VK_H			},	{"H",		VK_H		}},	// ID_KEY_36
	{{"J",		"J",	VK_J			},	{"J",		VK_J		}},	// ID_KEY_37
	{{"K",		"K",	VK_K			},	{"K",		VK_K		}},	// ID_KEY_38
	{{"L",		"L",	VK_L			},	{"L",		VK_L		}},	// ID_KEY_39
	{{";",		"+",	VK_OEM_PLUS		},	{"",		VK_NONE_00	}},	// ID_KEY_40
	{{":",		"*",	VK_OEM_1		},	{"Pause",	VK_PAUSE	}},	// ID_KEY_41
	{{"]",		"}",	VK_OEM_6		},	{"PrtS",	VK_SNAPSHOT	}},	// ID_KEY_42

	{{"Shift",	"Shift",VK_LSHIFT		},	{"Shift",	VK_LSHIFT	}},	// ID_KEY_43
	{{"Z",		"Z",	VK_Z			},	{"Z",		VK_A		}},	// ID_KEY_44
	{{"X",		"X",	VK_X			},	{"X",		VK_Z		}},	// ID_KEY_45
	{{"C",		"C",	VK_C			},	{"C",		VK_C		}},	// ID_KEY_46
	{{"V",		"V",	VK_V			},	{"V",		VK_V		}},	// ID_KEY_47
	{{"B",		"B",	VK_B			},	{"B",		VK_B		}},	// ID_KEY_48
	{{"N",		"N",	VK_N			},	{"N",		VK_N		}},	// ID_KEY_49
	{{"M",		"M",	VK_M			},	{"M",		VK_M		}},	// ID_KEY_50
	{{",",		"<",	VK_OEM_COMMA	},	{"",		VK_NONE_00	}},	// ID_KEY_51
	{{".",		">",	VK_OEM_PERIOD	},	{"",		VK_NONE_00	}},	// ID_KEY_52
	{{"/",		"?",	VK_OEM_2		},	{"",		VK_NONE_00	}},	// ID_KEY_53
	{{"\\",		"_",	VK_OEM_102		},	{"",		VK_NONE_00	}},	// ID_KEY_54
	{{"↑",		"↑",	VK_UP			},	{"PgU",		VK_PRIOR	}},	// ID_KEY_55
	{{"Shift",	"Shift",VK_RSHIFT		},	{"Shift",	VK_RSHIFT	}},	// ID_KEY_56

	{{"Fn",		"Fn",	VK_NONE_00		},	{"Fn",		VK_NONE_00	}},	// ID_KEY_57
	{{"Alt",	"Alt",	VK_LMENU		},	{"Alt",		VK_LMENU	}},	// ID_KEY_58
	{{"Win",	"Win",	VK_LWIN			},	{"Win",		VK_LWIN		}},	// ID_KEY_59
	{{"JP",		"JP",	VK_KANJI		},	{"JP",		VK_KANJI	}},	// ID_KEY_60
	{{"Space",	"Space",VK_SPACE		},	{"Space",	VK_SPACE	}},	// ID_KEY_61
	{{"Ctrl",	"Ctrl",	VK_RCONTROL		},	{"Ctrl",	VK_RCONTROL	}},	// ID_KEY_62
	{{"Alt",	"Alt",	VK_RMENU		},	{"Alt",		VK_RMENU	}},	// ID_KEY_63
	{{"Fn",		"Fn",	VK_NONE_00		},	{"Fn",		VK_NONE_00	}},	// ID_KEY_64
	{{"←",		"←",	VK_LEFT			},	{"Home",	VK_HOME		}},	// ID_KEY_65
	{{"↓",		"↓",	VK_DOWN			},	{"PgD",		VK_NEXT		}},	// ID_KEY_66
	{{"→",		"→",	VK_RIGHT		},	{"End",		VK_END		}},	// ID_KEY_67
  };

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
SoftwareKeyboard_JP::SoftwareKeyboard_JP(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent)
  :
  SoftwareKeyboard(settings, keyBuffer, parent)
{
  // initialize key top text
  initializeKeyTopText();
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed key
void SoftwareKeyboard_JP::pressedKey(ID_KEY id)
{
  if (outputLog){
	cout << "Pressed Key! id = " << id << endl << flush;
	if (onShiftKey)
	  cout << "Pressed : " << keyTopInfo[id].keyTop.keyTopTextWithShift << endl << flush;
	else if (onFnKey)
	  cout << "Pressed : " << keyTopInfo[id].keyTopWithFn.keyTopText << endl << flush;
	else
	  cout << "Pressed : " << keyTopInfo[id].keyTop.keyTopText << endl << flush;
  }

  // check connected
  if (!settings->getConnected()){
	//	return;
  }

  // check id
  if (id <= 0 || id > ID_KEY_NUM){
	// error
	return;
  }

  switch(id){
  case ID_KEY_43:
  case ID_KEY_56:
	// Shift
	onShiftKey = !onShiftKey;
	cout << "onShiftKey : " << onShiftKey << endl << flush;
	pressedShiftKey();
	break;
  case ID_KEY_30:
  case ID_KEY_62:
	// Control
	onControlKey = !onControlKey;
	cout << "onControlKey : " << onControlKey << endl << flush;
	break;
  case ID_KEY_58:
  case ID_KEY_63:
	// Alt
	onAltKey = !onAltKey;
	cout << "onAltKey : " << onAltKey << endl << flush;
	break;
  case ID_KEY_57:
  case ID_KEY_64:
	// Fn
	onFnKey = !onFnKey;
	cout << "onFnKey : " << onFnKey << endl << flush;
	pressedFnKey();
	break;
  default:
	if (onFnKey){
	  // Fn keys
	  char key = keyTopInfo[id].keyTopWithFn.VK_Code;
	  if (key != VK_NONE_00){
		keyBuffer->put(key, KEYCODE_FLG_KEYDOWN);
		if (outputLog){
		  cout << "VK_Code : " << getVKCodeByString(key)  << endl << flush;
		}
	  }
	}
	else { // except for Fn keys
	  if (onShiftKey){
		keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYDOWN);
	  }
	  if (onControlKey){
		keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYDOWN);
	  }
	  if (onAltKey){
		keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYDOWN);
	  }
	  char key = keyTopInfo[id].keyTop.VK_Code;
	  keyBuffer->put(key, KEYCODE_FLG_KEYDOWN);
	  if (outputLog){
		cout << "VK_Code : " << getVKCodeByString(key)  << endl << flush;
	  }
	}
	break;
  }
}

// released key
void SoftwareKeyboard_JP::releasedKey(ID_KEY id)
{
  if (outputLog){
	cout << "Released Key! id = " << id << endl << flush;
	if (onShiftKey)
	  cout << "Released : " << keyTopInfo[id].keyTop.keyTopTextWithShift << endl << flush;
	else if (onFnKey)
	  cout << "Released : " << keyTopInfo[id].keyTopWithFn.keyTopText << endl << flush;
	else
	  cout << "Released : " << keyTopInfo[id].keyTop.keyTopText << endl << flush;
  }

  // check id
  if (id <= 0 || id > ID_KEY_NUM){
	// error
	return;
  }

  // check connected
  if (!settings->getConnected()){
	//	return;
  }

  switch(id){
  case ID_KEY_43:
  case ID_KEY_56:
	// Shift
  case ID_KEY_30:
  case ID_KEY_62:
	// Control
  case ID_KEY_58:
  case ID_KEY_63:
	// Alt
  case ID_KEY_57:
  case ID_KEY_64:
	// Fn
	// Nothig to do
	break;
  default:
	if (onFnKey){
	  // Fn keys
	  char key = keyTopInfo[id].keyTopWithFn.VK_Code;
	  if (key != VK_NONE_00){
		keyBuffer->put(key, KEYCODE_FLG_KEYUP);
		if (outputLog){
		  cout << "VK_Code : " << getVKCodeByString(key)  << endl << flush;
		}
	  }
	}
	else { // except for Fn keys
	  char key = keyTopInfo[id].keyTop.VK_Code;
	  keyBuffer->put(key, KEYCODE_FLG_KEYUP);
	  if (outputLog){
		cout << "VK_Code : " << getVKCodeByString(key)  << endl << flush;
	  }
	  if (onAltKey){
		keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYUP);
		onAltKey = false;
	  }
	  if (onControlKey){
		keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYUP);
		onControlKey = false;
	  }
	  if (onShiftKey){
		keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYUP);
		onShiftKey = false;
	  }
	}
	break;
  }
}

// update key top text
void SoftwareKeyboard_JP::initializeKeyTopText()
{
  for (int i = 1; i <= ID_KEY_NUM; i++){
	buttons[i]->setText(keyTopInfo[i].keyTop.keyTopText);
  }
}

// shift key
void SoftwareKeyboard_JP::pressedShiftKey()
{
  if (onShiftKey){
	// pressed
	for (int i = 1; i <= ID_KEY_NUM; i++){
	  buttons[i]->setText(keyTopInfo[i].keyTop.keyTopTextWithShift);
	}
  }
  else {
	// released
	initializeKeyTopText();
  }
}

// Fn key
void SoftwareKeyboard_JP::pressedFnKey()
{
  if (onFnKey){
	// pressed
	for (int i = 1; i <= ID_KEY_NUM; i++){
	  buttons[i]->setText(keyTopInfo[i].keyTopWithFn.keyTopText);
	}
  }
  else {
	// released
	initializeKeyTopText();
  }
}

} // end of namespace qtbrynhildr
