// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard_us.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_keyboard_us.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

  // key information table
  const static SoftwareKeyboard::KeyTopInfo keyTopInfo[SoftwareKeyboard::ID_KEY_NUM+1] = {
	{{"",		"",		(char)VK_NONE_00	},	{"",		(char)VK_NONE_00	}},	// DUMMY

	{{"ESC",	"ESC",	(char)VK_ESCAPE		},	{"ESC",		(char)VK_ESCAPE		}},	// ID_KEY_1
	{{"1",		"!",	(char)VK_1			},	{"F1",		(char)VK_F1			}},	// ID_KEY_2
	{{"2",		"@",	(char)VK_2			},	{"F2",		(char)VK_F2			}},	// ID_KEY_3
	{{"3",		"#",	(char)VK_3			},	{"F3",		(char)VK_F3			}},	// ID_KEY_4
	{{"4",		"$",	(char)VK_4			},	{"F4",		(char)VK_F4			}},	// ID_KEY_5
	{{"5",		"%",	(char)VK_5			},	{"F5",		(char)VK_F5			}},	// ID_KEY_6
	{{"6",		"^",	(char)VK_6			},	{"F6",		(char)VK_F6			}},	// ID_KEY_7
	{{"7",		"&&",	(char)VK_7			},	{"F7",		(char)VK_F7			}},	// ID_KEY_8
	{{"8",		"*",	(char)VK_8			},	{"F8",		(char)VK_F8			}},	// ID_KEY_9
	{{"9",		"(",	(char)VK_9			},	{"F9",		(char)VK_F9			}},	// ID_KEY_10
	{{"0",		")",	(char)VK_0			},	{"F10",		(char)VK_F10		}},	// ID_KEY_11
	{{"-",		"_",	(char)VK_OEM_MINUS	},	{"F11",		(char)VK_F11		}},	// ID_KEY_12
	{{"=",		"+",	(char)VK_OEM_7		},	{"F12",		(char)VK_F12		}},	// ID_KEY_13
	{{"",		"",		(char)VK_NONE_00	},	{"Ins",		(char)VK_INSERT		}},	// ID_KEY_14
	{{"BS",		"BS",	(char)VK_BACK		},	{"Del",		(char)VK_DELETE		}},	// ID_KEY_15

	{{"Tab",	"Tab",	(char)VK_TAB		},	{"Tab",		(char)VK_TAB		}},	// ID_KEY_16
	{{"Q",		"Q",	(char)VK_Q			},	{"Q",		(char)VK_Q			}},	// ID_KEY_17
	{{"W",		"W",	(char)VK_W			},	{"W",		(char)VK_W			}},	// ID_KEY_18
	{{"E",		"E",	(char)VK_E			},	{"E",		(char)VK_E			}},	// ID_KEY_19
	{{"R",		"R",	(char)VK_R			},	{"R",		(char)VK_R			}},	// ID_KEY_20
	{{"T",		"T",	(char)VK_T			},	{"T",		(char)VK_T			}},	// ID_KEY_21
	{{"Y",		"Y",	(char)VK_Y			},	{"Y",		(char)VK_Y			}},	// ID_KEY_22
	{{"U",		"U",	(char)VK_U			},	{"U",		(char)VK_U			}},	// ID_KEY_23
	{{"I",		"I",	(char)VK_I			},	{"I",		(char)VK_I			}},	// ID_KEY_24
	{{"O",		"O",	(char)VK_O			},	{"O",		(char)VK_O			}},	// ID_KEY_25
	{{"P",		"P",	(char)VK_P			},	{"P",		(char)VK_P			}},	// ID_KEY_26
	{{"[",		"{",	(char)VK_OEM_3		},	{"",		(char)VK_NONE_00	}},	// ID_KEY_27
	{{"]",		"}",	(char)VK_OEM_4		},	{"",		(char)VK_NONE_00	}},	// ID_KEY_28
	{{"RET",	"RET",	(char)VK_RETURN		},	{"RET",		(char)VK_RETURN		}},	// ID_KEY_29

	{{"Ctrl",	"Ctrl",	(char)VK_LCONTROL	},	{"Ctrl",	(char)VK_LCONTROL	}},	// ID_KEY_30
	{{"A",		"A",	(char)VK_A			},	{"A",		(char)VK_A			}},	// ID_KEY_31
	{{"S",		"S",	(char)VK_S			},	{"S",		(char)VK_S			}},	// ID_KEY_32
	{{"D",		"D",	(char)VK_D			},	{"D",		(char)VK_D			}},	// ID_KEY_33
	{{"F",		"F",	(char)VK_F			},	{"F",		(char)VK_F			}},	// ID_KEY_34
	{{"G",		"G",	(char)VK_G			},	{"G",		(char)VK_G			}},	// ID_KEY_35
	{{"H",		"H",	(char)VK_H			},	{"H",		(char)VK_H			}},	// ID_KEY_36
	{{"J",		"J",	(char)VK_J			},	{"J",		(char)VK_J			}},	// ID_KEY_37
	{{"K",		"K",	(char)VK_K			},	{"K",		(char)VK_K			}},	// ID_KEY_38
	{{"L",		"L",	(char)VK_L			},	{"L",		(char)VK_L			}},	// ID_KEY_39
	{{";",		":",	(char)VK_OEM_PLUS	},	{"",		(char)VK_NONE_00	}},	// ID_KEY_40
	{{"'",		"\"",	(char)VK_OEM_1		},	{"Pause",	(char)VK_PAUSE		}},	// ID_KEY_41
	{{"\\",		"|",	(char)VK_OEM_6		},	{"PrtS",	(char)VK_SNAPSHOT	}},	// ID_KEY_42

	{{"Shift",	"Shift",(char)VK_LSHIFT		},	{"Shift",	(char)VK_LSHIFT		}},	// ID_KEY_43
	{{"Z",		"Z",	(char)VK_Z			},	{"Z",		(char)VK_A			}},	// ID_KEY_44
	{{"X",		"X",	(char)VK_X			},	{"X",		(char)VK_Z			}},	// ID_KEY_45
	{{"C",		"C",	(char)VK_C			},	{"C",		(char)VK_C			}},	// ID_KEY_46
	{{"V",		"V",	(char)VK_V			},	{"V",		(char)VK_V			}},	// ID_KEY_47
	{{"B",		"B",	(char)VK_B			},	{"B",		(char)VK_B			}},	// ID_KEY_48
	{{"N",		"N",	(char)VK_N			},	{"N",		(char)VK_N			}},	// ID_KEY_49
	{{"M",		"M",	(char)VK_M			},	{"M",		(char)VK_M			}},	// ID_KEY_50
	{{",",		"<",	(char)VK_OEM_COMMA	},	{"",		(char)VK_NONE_00	}},	// ID_KEY_51
	{{".",		">",	(char)VK_OEM_PERIOD	},	{"",		(char)VK_NONE_00	}},	// ID_KEY_52
	{{"\\",		"?",	(char)VK_OEM_2		},	{"",		(char)VK_NONE_00	}},	// ID_KEY_53
	{{"",		"",		(char)VK_NONE_00	},	{"",		(char)VK_NONE_00	}},	// ID_KEY_54
	{{"↑",		"↑",	(char)VK_UP			},	{"PgU",		(char)VK_PRIOR		}},	// ID_KEY_55
	{{"Shift",	"Shift",(char)VK_RSHIFT		},	{"Shift",	(char)VK_RSHIFT		}},	// ID_KEY_56

	{{"Fn",		"Fn",	(char)VK_NONE_00	},	{"Fn",		(char)VK_NONE_00	}},	// ID_KEY_57
	{{"Alt",	"Alt",	(char)VK_LMENU		},	{"Alt",		(char)VK_LMENU		}},	// ID_KEY_58
	{{"Win",	"Win",	(char)VK_LWIN		},	{"Win",		(char)VK_LWIN		}},	// ID_KEY_59
	{{"`",		"~",	(char)VK_OEM_7		},	{"`",		(char)VK_OEM_7		}},	// ID_KEY_60
	{{"Space",	"Space",(char)VK_SPACE		},	{"Space",	(char)VK_SPACE		}},	// ID_KEY_61
	{{"Ctrl",	"Ctrl",	(char)VK_RCONTROL	},	{"Ctrl",	(char)VK_RCONTROL	}},	// ID_KEY_62
	{{"Alt",	"Alt",	(char)VK_RMENU		},	{"Alt",		(char)VK_RMENU		}},	// ID_KEY_63
	{{"Fn",		"Fn",	(char)VK_NONE_00	},	{"Fn",		(char)VK_NONE_00	}},	// ID_KEY_64
	{{"←",		"←",	(char)VK_LEFT		},	{"Home",	(char)VK_HOME		}},	// ID_KEY_65
	{{"↓",		"↓",	(char)VK_DOWN		},	{"PgD",		(char)VK_NEXT		}},	// ID_KEY_66
	{{"→",		"→",	(char)VK_RIGHT		},	{"End",		(char)VK_END		}},	// ID_KEY_67
  };

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
SoftwareKeyboard_US::SoftwareKeyboard_US(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent)
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
void SoftwareKeyboard_US::pressedKey(ID_KEY id)
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
void SoftwareKeyboard_US::releasedKey(ID_KEY id)
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
void SoftwareKeyboard_US::initializeKeyTopText()
{
  for (int i = 1; i <= ID_KEY_NUM; i++){
	buttons[i]->setText(keyTopInfo[i].keyTop.keyTopText);
  }
}

// shift key
void SoftwareKeyboard_US::pressedShiftKey()
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
void SoftwareKeyboard_US::pressedFnKey()
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
