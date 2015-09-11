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
	{{(char*)"",		(char*)"",		(char)VK_NONE_00	},	{(char*)"",			(char)VK_NONE_00	}},	// DUMMY

	{{(char*)"ESC",		(char*)"ESC",	(char)VK_ESCAPE		},	{(char*)"ESC",		(char)VK_ESCAPE		}},	// ID_KEY_1
	{{(char*)"1",		(char*)"!",		(char)VK_1			},	{(char*)"F1",		(char)VK_F1			}},	// ID_KEY_2
	{{(char*)"2",		(char*)"\"",	(char)VK_2			},	{(char*)"F2",		(char)VK_F2			}},	// ID_KEY_3
	{{(char*)"3",		(char*)"#",		(char)VK_3			},	{(char*)"F3",		(char)VK_F3			}},	// ID_KEY_4
	{{(char*)"4",		(char*)"$",		(char)VK_4			},	{(char*)"F4",		(char)VK_F4			}},	// ID_KEY_5
	{{(char*)"5",		(char*)"%",		(char)VK_5			},	{(char*)"F5",		(char)VK_F5			}},	// ID_KEY_6
	{{(char*)"6",		(char*)"&&",	(char)VK_6			},	{(char*)"F6",		(char)VK_F6			}},	// ID_KEY_7
	{{(char*)"7",		(char*)"'",		(char)VK_7			},	{(char*)"F7",		(char)VK_F7			}},	// ID_KEY_8
	{{(char*)"8",		(char*)"(",		(char)VK_8			},	{(char*)"F8",		(char)VK_F8			}},	// ID_KEY_9
	{{(char*)"9",		(char*)")",		(char)VK_9			},	{(char*)"F9",		(char)VK_F9			}},	// ID_KEY_10
	{{(char*)"0",		(char*)"",		(char)VK_0			},	{(char*)"F10",		(char)VK_F10		}},	// ID_KEY_11
	{{(char*)"-",		(char*)"=",		(char)VK_OEM_MINUS	},	{(char*)"F11",		(char)VK_F11		}},	// ID_KEY_12
	{{(char*)"^",		(char*)"~",		(char)VK_OEM_7		},	{(char*)"F12",		(char)VK_F12		}},	// ID_KEY_13
	{{(char*)"\\",		(char*)"|",		(char)VK_OEM_5		},	{(char*)"Ins",		(char)VK_INSERT		}},	// ID_KEY_14
	{{(char*)"BS",		(char*)"BS",	(char)VK_BACK		},	{(char*)"Del",		(char)VK_DELETE		}},	// ID_KEY_15

	{{(char*)"Tab",		(char*)"Tab",	(char)VK_TAB		},	{(char*)"Tab",		(char)VK_TAB		}},	// ID_KEY_16
	{{(char*)"Q",		(char*)"Q",		(char)VK_Q			},	{(char*)"Q",		(char)VK_Q			}},	// ID_KEY_17
	{{(char*)"W",		(char*)"W",		(char)VK_W			},	{(char*)"W",		(char)VK_W			}},	// ID_KEY_18
	{{(char*)"E",		(char*)"E",		(char)VK_E			},	{(char*)"E",		(char)VK_E			}},	// ID_KEY_19
	{{(char*)"R",		(char*)"R",		(char)VK_R			},	{(char*)"R",		(char)VK_R			}},	// ID_KEY_20
	{{(char*)"T",		(char*)"T",		(char)VK_T			},	{(char*)"T",		(char)VK_T			}},	// ID_KEY_21
	{{(char*)"Y",		(char*)"Y",		(char)VK_Y			},	{(char*)"Y",		(char)VK_Y			}},	// ID_KEY_22
	{{(char*)"U",		(char*)"U",		(char)VK_U			},	{(char*)"U",		(char)VK_U			}},	// ID_KEY_23
	{{(char*)"I",		(char*)"I",		(char)VK_I			},	{(char*)"I",		(char)VK_I			}},	// ID_KEY_24
	{{(char*)"O",		(char*)"O",		(char)VK_O			},	{(char*)"O",		(char)VK_O			}},	// ID_KEY_25
	{{(char*)"P",		(char*)"P",		(char)VK_P			},	{(char*)"P",		(char)VK_P			}},	// ID_KEY_26
	{{(char*)"@",		(char*)"`",		(char)VK_OEM_3		},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_27
	{{(char*)"[",		(char*)"{",		(char)VK_OEM_4		},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_28
	{{(char*)"RET",		(char*)"RET",	(char)VK_RETURN		},	{(char*)"RET",		(char)VK_RETURN		}},	// ID_KEY_29

	{{(char*)"Ctrl",	(char*)"Ctrl",	(char)VK_LCONTROL	},	{(char*)"Ctrl",		(char)VK_LCONTROL	}},	// ID_KEY_30
	{{(char*)"A",		(char*)"A",		(char)VK_A			},	{(char*)"A",		(char)VK_A			}},	// ID_KEY_31
	{{(char*)"S",		(char*)"S",		(char)VK_S			},	{(char*)"S",		(char)VK_S			}},	// ID_KEY_32
	{{(char*)"D",		(char*)"D",		(char)VK_D			},	{(char*)"D",		(char)VK_D			}},	// ID_KEY_33
	{{(char*)"F",		(char*)"F",		(char)VK_F			},	{(char*)"F",		(char)VK_F			}},	// ID_KEY_34
	{{(char*)"G",		(char*)"G",		(char)VK_G			},	{(char*)"G",		(char)VK_G			}},	// ID_KEY_35
	{{(char*)"H",		(char*)"H",		(char)VK_H			},	{(char*)"H",		(char)VK_H			}},	// ID_KEY_36
	{{(char*)"J",		(char*)"J",		(char)VK_J			},	{(char*)"J",		(char)VK_J			}},	// ID_KEY_37
	{{(char*)"K",		(char*)"K",		(char)VK_K			},	{(char*)"K",		(char)VK_K			}},	// ID_KEY_38
	{{(char*)"L",		(char*)"L",		(char)VK_L			},	{(char*)"L",		(char)VK_L			}},	// ID_KEY_39
	{{(char*)";",		(char*)"+",		(char)VK_OEM_PLUS	},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_40
	{{(char*)":",		(char*)"*",		(char)VK_OEM_1		},	{(char*)"Pause",	(char)VK_PAUSE		}},	// ID_KEY_41
	{{(char*)"]",		(char*)"}",		(char)VK_OEM_6		},	{(char*)"PrtS",		(char)VK_SNAPSHOT	}},	// ID_KEY_42

	{{(char*)"Shift",	(char*)"Shift",	(char)VK_LSHIFT		},	{(char*)"Shift",	(char)VK_LSHIFT		}},	// ID_KEY_43
	{{(char*)"Z",		(char*)"Z",		(char)VK_Z			},	{(char*)"Z",		(char)VK_A			}},	// ID_KEY_44
	{{(char*)"X",		(char*)"X",		(char)VK_X			},	{(char*)"X",		(char)VK_Z			}},	// ID_KEY_45
	{{(char*)"C",		(char*)"C",		(char)VK_C			},	{(char*)"C",		(char)VK_C			}},	// ID_KEY_46
	{{(char*)"V",		(char*)"V",		(char)VK_V			},	{(char*)"V",		(char)VK_V			}},	// ID_KEY_47
	{{(char*)"B",		(char*)"B",		(char)VK_B			},	{(char*)"B",		(char)VK_B			}},	// ID_KEY_48
	{{(char*)"N",		(char*)"N",		(char)VK_N			},	{(char*)"N",		(char)VK_N			}},	// ID_KEY_49
	{{(char*)"M",		(char*)"M",		(char)VK_M			},	{(char*)"M",		(char)VK_M			}},	// ID_KEY_50
	{{(char*)",",		(char*)"<",		(char)VK_OEM_COMMA	},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_51
	{{(char*)".",		(char*)">",		(char)VK_OEM_PERIOD	},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_52
	{{(char*)"/",		(char*)"?",		(char)VK_OEM_2		},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_53
	{{(char*)"\\",		(char*)"_",		(char)VK_OEM_102	},	{(char*)"",			(char)VK_NONE_00	}},	// ID_KEY_54
	{{(char*)"↑",		(char*)"↑",	(char)VK_UP			},	{(char*)"PgU",		(char)VK_PRIOR		}},	// ID_KEY_55
	{{(char*)"Shift",	(char*)"Shift",	(char)VK_RSHIFT		},	{(char*)"Shift",	(char)VK_RSHIFT		}},	// ID_KEY_56

	{{(char*)"Fn",		(char*)"Fn",	(char)VK_NONE_00	},	{(char*)"Fn",		(char)VK_NONE_00	}},	// ID_KEY_57
	{{(char*)"Alt",		(char*)"Alt",	(char)VK_LMENU		},	{(char*)"Alt",		(char)VK_LMENU		}},	// ID_KEY_58
	{{(char*)"Win",		(char*)"Win",	(char)VK_LWIN		},	{(char*)"Win",		(char)VK_LWIN		}},	// ID_KEY_59
	{{(char*)"JP",		(char*)"JP",	(char)VK_KANJI		},	{(char*)"JP",		(char)VK_KANJI		}},	// ID_KEY_60
	{{(char*)"Space",	(char*)"Space",	(char)VK_SPACE		},	{(char*)"Space",	(char)VK_SPACE		}},	// ID_KEY_61
	{{(char*)"Ctrl",	(char*)"Ctrl",	(char)VK_RCONTROL	},	{(char*)"Ctrl",		(char)VK_RCONTROL	}},	// ID_KEY_62
	{{(char*)"Alt",		(char*)"Alt",	(char)VK_RMENU		},	{(char*)"Alt",		(char)VK_RMENU		}},	// ID_KEY_63
	{{(char*)"Fn",		(char*)"Fn",	(char)VK_NONE_00	},	{(char*)"Fn",		(char)VK_NONE_00	}},	// ID_KEY_64
	{{(char*)"←",		(char*)"←",	(char)VK_LEFT		},	{(char*)"Home",		(char)VK_HOME		}},	// ID_KEY_65
	{{(char*)"↓",		(char*)"↓",	(char)VK_DOWN		},	{(char*)"PgD",		(char)VK_NEXT		}},	// ID_KEY_66
	{{(char*)"→",		(char*)"→",	(char)VK_RIGHT		},	{(char*)"End",		(char)VK_END		}},	// ID_KEY_67
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
