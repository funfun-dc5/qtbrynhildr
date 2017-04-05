// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SOFTWARE_KEYBOARD_H
#define SOFTWARE_KEYBOARD_H

// System Header
#include <string>

// Qt Header
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>

// Local Header
#include "config.h"
#include "windows/keycodes.h"

#if USE_LAYOUTFILE
#include "layoutfile_reader.h"
#endif // USE_LAYOUTFILE

using namespace std;

namespace qtbrynhildr {

// SoftwareKeyboard
class SoftwareKeyboard : public QWidget
{
#if USE_LAYOUTFILE
  friend LayoutFileReader;
#endif // USE_LAYOUTFILE

public:
  // keyboard type
  typedef enum {
	KEYTOP_TYPE_JP,
	KEYTOP_TYPE_US
  } KEYTOP_TYPE;

public:
  // constructor
  SoftwareKeyboard(QWidget *parent = 0);
  SoftwareKeyboard(KEYTOP_TYPE type, QWidget *parent = 0);
  // destructor
  //  ~SoftwareKeyboard();

#if 1 // for TEST
  // get keytop type
  KEYTOP_TYPE getKeytopType();

  // get keytop type name
  QString getKeytopTypeName();

  // set keytop type
  void setKeytopType(KEYTOP_TYPE type);
#endif

  // reset size
  QSize resetSize();

protected:
  // paint event
  void paintEvent(QPaintEvent *event) override;

  // resize event
  void resizeEvent(QResizeEvent *event) override;

#if 0
  // minimum size hint
  QSize minimumSizeHint() const override;
#endif

  // size hint
  QSize sizeHint() const override;

  // mouse event
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  // key down
  virtual void keyDown(uchar key);
  
  // key up
  virtual void keyUp(uchar key);

private:
  // name table of virtual keycode for windows
  string stringTableOfVKCode[256] = {
	// 00 - 0F
	"VK_NONE_00",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"VK_NONE_07",
	"VK_BACK",
	"VK_TAB",
	"VK_NONE_0A",
	"VK_NONE_0B",
	"VK_CLEAR",
	"VK_RETURN",
	"VK_NONE_0E",
	"VK_NONE_0F",

	// 10 - 1F
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA/HANGUL",
	"VK_NONE_16",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI/HANJI",
	"VK_NONE_1A",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",

	// 20 - 2F
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_NONE_2A",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",

	// 30 - 3F
	"VK_0",
	"VK_1",
	"VK_2",
	"VK_3",
	"VK_4",
	"VK_5",
	"VK_6",
	"VK_7",
	"VK_8",
	"VK_9",
	"VK_NONE_3A",
	"VK_NONE_3B",
	"VK_NONE_3C",
	"VK_NONE_3D",
	"VK_NONE_3E",
	"VK_NONE_3F",

	// 40 - 4F
	"VK_NONE_40",
	"VK_A",
	"VK_B",
	"VK_C",
	"VK_D",
	"VK_E",
	"VK_F",
	"VK_G",
	"VK_H",
	"VK_I",
	"VK_J",
	"VK_K",
	"VK_L",
	"VK_M",
	"VK_N",
	"VK_O",

	// 50 - 5F
	"VK_P",
	"VK_Q",
	"VK_R",
	"VK_S",
	"VK_T",
	"VK_U",
	"VK_V",
	"VK_W",
	"VK_X",
	"VK_Y",
	"VK_Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"VK_NONE_5E",
	"VK_SLEEP",

	// 60 - 6F
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",

	// 70 - 7F
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",

	// 80 - 8F
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"VK_NONE_88",
	"VK_NONE_89",
	"VK_NONE_8A",
	"VK_NONE_8B",
	"VK_NONE_8C",
	"VK_NONE_8D",
	"VK_NONE_8E",
	"VK_NONE_8F",

	// 90 - 9F
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_EQU",
	"VK_NONE_93",
	"VK_NONE_94",
	"VK_NONE_95",
	"VK_NONE_96",
	"VK_NONE_97",
	"VK_NONE_98",
	"VK_NONE_99",
	"VK_NONE_9A",
	"VK_NONE_9B",
	"VK_NONE_9C",
	"VK_NONE_9D",
	"VK_NONE_9E",
	"VK_NONE_9F",

	// A0 - AF
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU",
	"VK_BROWSER_BACK",
	"VK_BROWSER_FORWARD",
	"VK_BROWSER_REFRESH",
	"VK_BROWSER_STOP",
	"VK_BROWSER_SEARCH",
	"VK_BROWSER_FAVORITES",
	"VK_BROWSER_HOME",
	"VK_VOLUME_MUTE",
	"VK_VOLUME_DOWN",
	"VK_VOLUME_UP",

	// B0 - BF
	"VK_MEDIA_NEXT_TRACK",
	"VK_MEDIA_PREV_TRACK",
	"VK_MEDIA_STOP",
	"VK_MEDIA_PLAY_PAUSE",
	"VK_LAUNCH_MAIL",
	"VK_LAUNCH_MEDIA_SELECT",
	"VK_LAUNCH_APP1",
	"VK_LAUNCH_APP2",
	"VK_NONE_B8",
	"VK_NONE_B9",
	"VK_OEM_1",
	"VK_OEM_PLUS",
	"VK_OEM_COMMA",
	"VK_OEM_MINUS",
	"VK_OEM_PERIOD",
	"VK_OEM_2",
	"VK_OEM_3",

	// C0 - CF
	"VK_NONE_C1",
	"VK_NONE_C2",
	"VK_NONE_C3",
	"VK_NONE_C4",
	"VK_NONE_C5",
	"VK_NONE_C6",
	"VK_NONE_C7",
	"VK_NONE_C8",
	"VK_NONE_C9",
	"VK_NONE_CA",
	"VK_NONE_CB",
	"VK_NONE_CC",
	"VK_NONE_CD",
	"VK_NONE_CE",
	"VK_NONE_CF",

	// D0 - DF
	"VK_NONE_D0",
	"VK_NONE_D1",
	"VK_NONE_D2",
	"VK_NONE_D3",
	"VK_NONE_D4",
	"VK_NONE_D5",
	"VK_NONE_D6",
	"VK_NONE_D7",
	"VK_NONE_D8",
	"VK_NONE_D9",
	"VK_NONE_DA",
	"VK_OEM_4",
	"VK_OEM_5",
	"VK_OEM_6",
	"VK_OEM_7",
	"VK_OEM_8",

	// E0 - EF
	"VK_NONE_E0",
	"VK_OEM_AX",
	"VK_OEM_102",
	"VK_ICO_HELP",
	"VK_ICO_00",
	"VK_PROCESSKEY",
	"VK_ICO_CLEAR",
	"VK_PACKET",
	"VK_NONE_E8",
	"VK_OEM_RESET",
	"VK_OEM_JUMP",
	"VK_OEM_PA1",
	"VK_OEM_PA2",
	"VK_OEM_PA3",
	"VK_OEM_WSCTRL",
	"VK_OEM_CUSEL",

	// F0 - FF
	"VK_OEM_ATTN",
	"VK_OEM_FINISH",
	"VK_OEM_COPY",
	"VK_OEM_AUTO",
	"VK_OEM_ENLW",
	"VK_OEM_BACKTAB",
	"VK_ATTN",
	"VK_CRSEL",
	"VK_EXSEL",
	"VK_EREOF",
	"VK_PLAY",
	"VK_ZOOM",
	"VK_NONAME",
	"VK_PA1",
	"VK_OEM_CLEAR",
	"VK_NONE_FF"
  };

  // key id
  typedef enum {
	ID_KEY_0,
	ID_KEY_1,
	ID_KEY_2,
	ID_KEY_3,
	ID_KEY_4,
	ID_KEY_5,
	ID_KEY_6,
	ID_KEY_7,
	ID_KEY_8,
	ID_KEY_9,
	ID_KEY_10,
	ID_KEY_11,
	ID_KEY_12,
	ID_KEY_13,
	ID_KEY_14,
	ID_KEY_15,
	ID_KEY_16,
	ID_KEY_17,
	ID_KEY_18,
	ID_KEY_19,
	ID_KEY_20,
	ID_KEY_21,
	ID_KEY_22,
	ID_KEY_23,
	ID_KEY_24,
	ID_KEY_25,
	ID_KEY_26,
	ID_KEY_27,
	ID_KEY_28,
	ID_KEY_29,
	ID_KEY_30,
	ID_KEY_31,
	ID_KEY_32,
	ID_KEY_33,
	ID_KEY_34,
	ID_KEY_35,
	ID_KEY_36,
	ID_KEY_37,
	ID_KEY_38,
	ID_KEY_39,
	ID_KEY_40,
	ID_KEY_41,
	ID_KEY_42,
	ID_KEY_43,
	ID_KEY_44,
	ID_KEY_45,
	ID_KEY_46,
	ID_KEY_47,
	ID_KEY_48,
	ID_KEY_49,
	ID_KEY_50,
	ID_KEY_51,
	ID_KEY_52,
	ID_KEY_53,
	ID_KEY_54,
	ID_KEY_55,
	ID_KEY_56,
	ID_KEY_57,
	ID_KEY_58,
	ID_KEY_59,
	ID_KEY_60,
	ID_KEY_61,
	ID_KEY_62,
	ID_KEY_63,
	ID_KEY_64,
	ID_KEY_65,
	ID_KEY_66,
	ID_KEY_67,
	ID_KEY_NUM
  } ID_KEY;

private:
  // calculate layout
  void calculateLayout(qreal xFactor, qreal yFactor);

  // get ID
  ID_KEY getID(QPoint pos) const;

  // pressed key
  void pressedKey(ID_KEY id);

  // released key
  void releasedKey(ID_KEY id);

  // shift key
  uchar pressedShiftKey(ID_KEY id);

  // control key
  uchar pressedControlKey(ID_KEY id);

  // alt key
  uchar pressedAltKey(ID_KEY id);

  // Fn key
  uchar pressedFnKey(ID_KEY id);

  // get name of virtual keycode
  string getVKCodeByString(uchar vkcode) const;

private:
  // original size for layout
  static const int WIDTH = 270;
  static const int HEIGHT = 75;

  // initial factor
  const qreal INITIAL_XFACTOR = 4.0;
#if defined(QTB_DEV_TABLET)
  const qreal INITIAL_YFACTOR = 4.0*1.11; // for TEST Nexus 7 (1200/1080 = 1.11)
#else // defined(QTB_DEV_TABLET)
  const qreal INITIAL_YFACTOR = 4.0;
#endif // defined(QTB_DEV_TABLET)

  // layout table for keyboard
  const QRect keyLayout[ID_KEY_NUM] = {
	//       x,  y,  w,  h
	QRect(  0,  0,  0,  0), // dummy entry

	// 1st row
	QRect(  0,  0, 18, 15),
	QRect( 18,  0, 18, 15),
	QRect( 36,  0, 18, 15),
	QRect( 54,  0, 18, 15),
	QRect( 72,  0, 18, 15),
	QRect( 90,  0, 18, 15),
	QRect(108,  0, 18, 15),
	QRect(126,  0, 18, 15),
	QRect(144,  0, 18, 15),
	QRect(162,  0, 18, 15),
	QRect(180,  0, 18, 15),
	QRect(198,  0, 18, 15),
	QRect(216,  0, 18, 15),
	QRect(234,  0, 18, 15),
	QRect(252,  0, 18, 15),

	// 2nd row
	QRect(  0, 15, 24, 15),
	QRect( 24, 15, 18, 15),
	QRect( 42, 15, 18, 15),
	QRect( 60, 15, 18, 15),
	QRect( 78, 15, 18, 15),
	QRect( 96, 15, 18, 15),
	QRect(114, 15, 18, 15),
	QRect(132, 15, 18, 15),
	QRect(150, 15, 18, 15),
	QRect(168, 15, 18, 15),
	QRect(186, 15, 18, 15),
	QRect(204, 15, 18, 15),
	QRect(222, 15, 26, 15),
	QRect(248, 15, 22, 30),

	// 3rd row
	QRect(  0, 30, 32, 15),
	QRect( 32, 30, 18, 15),
	QRect( 50, 30, 18, 15),
	QRect( 68, 30, 18, 15),
	QRect( 86, 30, 18, 15),
	QRect(104, 30, 18, 15),
	QRect(122, 30, 18, 15),
	QRect(140, 30, 18, 15),
	QRect(158, 30, 18, 15),
	QRect(176, 30, 18, 15),
	QRect(194, 30, 18, 15),
	QRect(212, 30, 18, 15),
	QRect(230, 30, 18, 15),

	// 4th row
	QRect(  0, 45, 36, 15),
	QRect( 36, 45, 18, 15),
	QRect( 54, 45, 18, 15),
	QRect( 72, 45, 18, 15),
	QRect( 90, 45, 18, 15),
	QRect(108, 45, 18, 15),
	QRect(126, 45, 18, 15),
	QRect(144, 45, 18, 15),
	QRect(162, 45, 18, 15),
	QRect(180, 45, 18, 15),
	QRect(198, 45, 18, 15),
	QRect(216, 45, 18, 15),
	QRect(234, 45, 18, 15),
	QRect(252, 45, 18, 15),

	// 5th row
	QRect(  0, 60, 36, 15),
	QRect( 36, 60, 18, 15),
	QRect( 54, 60, 18, 15),
	QRect( 72, 60, 18, 15),
	QRect( 90, 60, 72, 15),
	QRect(162, 60, 18, 15),
	QRect(180, 60, 18, 15),
	QRect(198, 60, 18, 15),
	QRect(216, 60, 18, 15),
	QRect(234, 60, 18, 15),
	QRect(252, 60, 18, 15)
  };

  // layout table for keyboard
  typedef struct {
	QRect rect;
	bool pushed;
  } Layout;
  Layout layout[ID_KEY_NUM];

  // key top information
  typedef struct {
	struct {
	  char keyTop[10];
	  char keyTopWithShift[10];
	  uchar VK_Code;
	} keyTop;
	struct {
	  char keyTop[10];
	  uchar VK_Code;
	} keyTopWithFn;
  } KeyTopInfo;

  // key top information table
  const KeyTopInfo *keyTopInfo;

  // key top information table for JP
  const KeyTopInfo keyTopInfo_JP[ID_KEY_NUM] = {
	{{"",		"",		(uchar)VK_NONE_00	},	{"",		(uchar)VK_NONE_00	}},	// DUMMY

	{{"Esc",	"Esc",	(uchar)VK_ESCAPE	},	{"Esc",		(uchar)VK_ESCAPE	}},	// ID_KEY_1
	{{"1",		"!",	(uchar)VK_1			},	{"F1",		(uchar)VK_F1		}},	// ID_KEY_2
	{{"2",		"\"",	(uchar)VK_2			},	{"F2",		(uchar)VK_F2		}},	// ID_KEY_3
	{{"3",		"#",	(uchar)VK_3			},	{"F3",		(uchar)VK_F3		}},	// ID_KEY_4
	{{"4",		"$",	(uchar)VK_4			},	{"F4",		(uchar)VK_F4		}},	// ID_KEY_5
	{{"5",		"%",	(uchar)VK_5			},	{"F5",		(uchar)VK_F5		}},	// ID_KEY_6
	{{"6",		"&",	(uchar)VK_6			},	{"F6",		(uchar)VK_F6		}},	// ID_KEY_7
	{{"7",		"'",	(uchar)VK_7			},	{"F7",		(uchar)VK_F7		}},	// ID_KEY_8
	{{"8",		"(",	(uchar)VK_8			},	{"F8",		(uchar)VK_F8		}},	// ID_KEY_9
	{{"9",		")",	(uchar)VK_9			},	{"F9",		(uchar)VK_F9		}},	// ID_KEY_10
	{{"0",		"",		(uchar)VK_0			},	{"F10",		(uchar)VK_F10		}},	// ID_KEY_11
	{{"-",		"=",	(uchar)VK_OEM_MINUS	},	{"F11",		(uchar)VK_F11		}},	// ID_KEY_12
	{{"^",		"~",	(uchar)VK_OEM_7		},	{"F12",		(uchar)VK_F12		}},	// ID_KEY_13
	//	{{"\\",		"|",	(uchar)VK_OEM_5		},	{"Ins",		(uchar)VK_INSERT	}},	// ID_KEY_14
	//	{{"￥",		"|",	(uchar)VK_OEM_5		},	{"Ins",		(uchar)VK_INSERT	}},	// ID_KEY_14
	{{"\uffe5",		"|",	(uchar)VK_OEM_5		},	{"Ins",		(uchar)VK_INSERT	}},	// ID_KEY_14
	{{"BS",		"BS",	(uchar)VK_BACK		},	{"Del",		(uchar)VK_DELETE	}},	// ID_KEY_15

	{{"Tab",	"Tab",	(uchar)VK_TAB		},	{"Tab",		(uchar)VK_TAB		}},	// ID_KEY_16
	{{"Q",		"Q",	(uchar)VK_Q			},	{"Q",		(uchar)VK_Q			}},	// ID_KEY_17
	{{"W",		"W",	(uchar)VK_W			},	{"W",		(uchar)VK_W			}},	// ID_KEY_18
	{{"E",		"E",	(uchar)VK_E			},	{"E",		(uchar)VK_E			}},	// ID_KEY_19
	{{"R",		"R",	(uchar)VK_R			},	{"R",		(uchar)VK_R			}},	// ID_KEY_20
	{{"T",		"T",	(uchar)VK_T			},	{"T",		(uchar)VK_T			}},	// ID_KEY_21
	{{"Y",		"Y",	(uchar)VK_Y			},	{"Y",		(uchar)VK_Y			}},	// ID_KEY_22
	{{"U",		"U",	(uchar)VK_U			},	{"U",		(uchar)VK_U			}},	// ID_KEY_23
	{{"I",		"I",	(uchar)VK_I			},	{"I",		(uchar)VK_I			}},	// ID_KEY_24
	{{"O",		"O",	(uchar)VK_O			},	{"O",		(uchar)VK_O			}},	// ID_KEY_25
	{{"P",		"P",	(uchar)VK_P			},	{"P",		(uchar)VK_P			}},	// ID_KEY_26
	{{"@",		"`",	(uchar)VK_OEM_3		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_27
	{{"[",		"{",	(uchar)VK_OEM_4		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_28
	{{"Enter",	"Enter",(uchar)VK_RETURN	},	{"Enter",	(uchar)VK_RETURN	}},	// ID_KEY_29

	{{"Ctrl",	"Ctrl",	(uchar)VK_LCONTROL	},	{"Ctrl",	(uchar)VK_LCONTROL	}},	// ID_KEY_30
	{{"A",		"A",	(uchar)VK_A			},	{"A",		(uchar)VK_A			}},	// ID_KEY_31
	{{"S",		"S",	(uchar)VK_S			},	{"S",		(uchar)VK_S			}},	// ID_KEY_32
	{{"D",		"D",	(uchar)VK_D			},	{"D",		(uchar)VK_D			}},	// ID_KEY_33
	{{"F",		"F",	(uchar)VK_F			},	{"F",		(uchar)VK_F			}},	// ID_KEY_34
	{{"G",		"G",	(uchar)VK_G			},	{"G",		(uchar)VK_G			}},	// ID_KEY_35
	{{"H",		"H",	(uchar)VK_H			},	{"H",		(uchar)VK_H			}},	// ID_KEY_36
	{{"J",		"J",	(uchar)VK_J			},	{"J",		(uchar)VK_J			}},	// ID_KEY_37
	{{"K",		"K",	(uchar)VK_K			},	{"K",		(uchar)VK_K			}},	// ID_KEY_38
	{{"L",		"L",	(uchar)VK_L			},	{"L",		(uchar)VK_L			}},	// ID_KEY_39
	{{";",		"+",	(uchar)VK_OEM_PLUS	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_40
	{{":",		"*",	(uchar)VK_OEM_1		},	{"Pause",	(uchar)VK_PAUSE		}},	// ID_KEY_41
	{{"]",		"}",	(uchar)VK_OEM_6		},	{"PrtS",	(uchar)VK_SNAPSHOT	}},	// ID_KEY_42

	{{"Shift",	"Shift",(uchar)VK_LSHIFT	},	{"Shift",	(uchar)VK_LSHIFT	}},	// ID_KEY_43
	{{"Z",		"Z",	(uchar)VK_Z			},	{"Z",		(uchar)VK_A			}},	// ID_KEY_44
	{{"X",		"X",	(uchar)VK_X			},	{"X",		(uchar)VK_Z			}},	// ID_KEY_45
	{{"C",		"C",	(uchar)VK_C			},	{"C",		(uchar)VK_C			}},	// ID_KEY_46
	{{"V",		"V",	(uchar)VK_V			},	{"V",		(uchar)VK_V			}},	// ID_KEY_47
	{{"B",		"B",	(uchar)VK_B			},	{"B",		(uchar)VK_B			}},	// ID_KEY_48
	{{"N",		"N",	(uchar)VK_N			},	{"N",		(uchar)VK_N			}},	// ID_KEY_49
	{{"M",		"M",	(uchar)VK_M			},	{"M",		(uchar)VK_M			}},	// ID_KEY_50
	{{",",		"<",	(uchar)VK_OEM_COMMA	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_51
	{{".",		">",	(uchar)VK_OEM_PERIOD},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_52
	{{"/",		"?",	(uchar)VK_OEM_2		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_53
	{{"\\",		"_",	(uchar)VK_OEM_102	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_54
	//	{{"↑",		"↑",	(uchar)VK_UP		},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
	{{"\u2191",		"\u2192",	(uchar)VK_UP		},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
	{{"Shift",	"Shift",(uchar)VK_RSHIFT	},	{"Shift",	(uchar)VK_RSHIFT	}},	// ID_KEY_56

	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}},	// ID_KEY_57
	{{"Alt",	"Alt",	(uchar)VK_LMENU		},	{"Alt",		(uchar)VK_LMENU		}},	// ID_KEY_58
	{{"Win",	"Win",	(uchar)VK_LWIN		},	{"Win",		(uchar)VK_LWIN		}},	// ID_KEY_59
	{{"JP",		"JP",	(uchar)VK_KANJI		},	{"JP",		(uchar)VK_KANJI		}},	// ID_KEY_60
	{{"Space",	"Space",(uchar)VK_SPACE		},	{"Space",	(uchar)VK_SPACE		}},	// ID_KEY_61
	{{"Ctrl",	"Ctrl",	(uchar)VK_RCONTROL	},	{"Ctrl",	(uchar)VK_RCONTROL	}},	// ID_KEY_62
	{{"Alt",	"Alt",	(uchar)VK_RMENU		},	{"Alt",		(uchar)VK_RMENU		}},	// ID_KEY_63
	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}},	// ID_KEY_64
	//	{{"←",		"←",	(uchar)VK_LEFT		},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	//	{{"↓",		"↓",	(uchar)VK_DOWN		},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	//	{{"→",		"→",	(uchar)VK_RIGHT		},	{"End",		(uchar)VK_END		}}	// ID_KEY_67
	{{"\u2190",	"\u2190",	(uchar)VK_LEFT		},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	{{"\u2193",	"\u2193",	(uchar)VK_DOWN		},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	{{"\u2192",	"\u2192",	(uchar)VK_RIGHT		},	{"End",		(uchar)VK_END		}}	// ID_KEY_67
  };

  // key top information table for US
  const KeyTopInfo keyTopInfo_US[ID_KEY_NUM] = {
	{{"",		"",		(uchar)VK_NONE_00	},	{"",		(uchar)VK_NONE_00	}},	// DUMMY

	{{"Esc",	"Esc",	(uchar)VK_ESCAPE	},	{"Esc",		(uchar)VK_ESCAPE	}},	// ID_KEY_1
	{{"1",		"!",	(uchar)VK_1			},	{"F1",		(uchar)VK_F1		}},	// ID_KEY_2
	{{"2",		"@",	(uchar)VK_2			},	{"F2",		(uchar)VK_F2		}},	// ID_KEY_3
	{{"3",		"#",	(uchar)VK_3			},	{"F3",		(uchar)VK_F3		}},	// ID_KEY_4
	{{"4",		"$",	(uchar)VK_4			},	{"F4",		(uchar)VK_F4		}},	// ID_KEY_5
	{{"5",		"%",	(uchar)VK_5			},	{"F5",		(uchar)VK_F5		}},	// ID_KEY_6
	{{"6",		"^",	(uchar)VK_6			},	{"F6",		(uchar)VK_F6		}},	// ID_KEY_7
	{{"7",		"&",	(uchar)VK_7			},	{"F7",		(uchar)VK_F7		}},	// ID_KEY_8
	{{"8",		"*",	(uchar)VK_8			},	{"F8",		(uchar)VK_F8		}},	// ID_KEY_9
	{{"9",		"(",	(uchar)VK_9			},	{"F9",		(uchar)VK_F9		}},	// ID_KEY_10
	{{"0",		")",	(uchar)VK_0			},	{"F10",		(uchar)VK_F10		}},	// ID_KEY_11
	{{"-",		"_",	(uchar)VK_OEM_MINUS	},	{"F11",		(uchar)VK_F11		}},	// ID_KEY_12
	{{"=",		"+",	(uchar)VK_OEM_7		},	{"F12",		(uchar)VK_F12		}},	// ID_KEY_13
	{{"",		"",		(uchar)VK_NONE_00	},	{"Ins",		(uchar)VK_INSERT	}},	// ID_KEY_14
	{{"BS",		"BS",	(uchar)VK_BACK		},	{"Del",		(uchar)VK_DELETE	}},	// ID_KEY_15

	{{"Tab",	"Tab",	(uchar)VK_TAB		},	{"Tab",		(uchar)VK_TAB		}},	// ID_KEY_16
	{{"Q",		"Q",	(uchar)VK_Q			},	{"Q",		(uchar)VK_Q			}},	// ID_KEY_17
	{{"W",		"W",	(uchar)VK_W			},	{"W",		(uchar)VK_W			}},	// ID_KEY_18
	{{"E",		"E",	(uchar)VK_E			},	{"E",		(uchar)VK_E			}},	// ID_KEY_19
	{{"R",		"R",	(uchar)VK_R			},	{"R",		(uchar)VK_R			}},	// ID_KEY_20
	{{"T",		"T",	(uchar)VK_T			},	{"T",		(uchar)VK_T			}},	// ID_KEY_21
	{{"Y",		"Y",	(uchar)VK_Y			},	{"Y",		(uchar)VK_Y			}},	// ID_KEY_22
	{{"U",		"U",	(uchar)VK_U			},	{"U",		(uchar)VK_U			}},	// ID_KEY_23
	{{"I",		"I",	(uchar)VK_I			},	{"I",		(uchar)VK_I			}},	// ID_KEY_24
	{{"O",		"O",	(uchar)VK_O			},	{"O",		(uchar)VK_O			}},	// ID_KEY_25
	{{"P",		"P",	(uchar)VK_P			},	{"P",		(uchar)VK_P			}},	// ID_KEY_26
	{{"[",		"{",	(uchar)VK_OEM_3		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_27
	{{"]",		"}",	(uchar)VK_OEM_4		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_28
	{{"Enter",	"Enter",(uchar)VK_RETURN	},	{"Enter",	(uchar)VK_RETURN	}},	// ID_KEY_29

	{{"Ctrl",	"Ctrl",	(uchar)VK_LCONTROL	},	{"Ctrl",	(uchar)VK_LCONTROL	}},	// ID_KEY_30
	{{"A",		"A",	(uchar)VK_A			},	{"A",		(uchar)VK_A			}},	// ID_KEY_31
	{{"S",		"S",	(uchar)VK_S			},	{"S",		(uchar)VK_S			}},	// ID_KEY_32
	{{"D",		"D",	(uchar)VK_D			},	{"D",		(uchar)VK_D			}},	// ID_KEY_33
	{{"F",		"F",	(uchar)VK_F			},	{"F",		(uchar)VK_F			}},	// ID_KEY_34
	{{"G",		"G",	(uchar)VK_G			},	{"G",		(uchar)VK_G			}},	// ID_KEY_35
	{{"H",		"H",	(uchar)VK_H			},	{"H",		(uchar)VK_H			}},	// ID_KEY_36
	{{"J",		"J",	(uchar)VK_J			},	{"J",		(uchar)VK_J			}},	// ID_KEY_37
	{{"K",		"K",	(uchar)VK_K			},	{"K",		(uchar)VK_K			}},	// ID_KEY_38
	{{"L",		"L",	(uchar)VK_L			},	{"L",		(uchar)VK_L			}},	// ID_KEY_39
	{{";",		":",	(uchar)VK_OEM_PLUS	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_40
	{{"'",		"\"",	(uchar)VK_OEM_1		},	{"Pause",	(uchar)VK_PAUSE		}},	// ID_KEY_41
	{{"\\",		"|",	(uchar)VK_OEM_6		},	{"PrtS",	(uchar)VK_SNAPSHOT	}},	// ID_KEY_42

	{{"Shift",	"Shift",(uchar)VK_LSHIFT	},	{"Shift",	(uchar)VK_LSHIFT	}},	// ID_KEY_43
	{{"Z",		"Z",	(uchar)VK_Z			},	{"Z",		(uchar)VK_A			}},	// ID_KEY_44
	{{"X",		"X",	(uchar)VK_X			},	{"X",		(uchar)VK_Z			}},	// ID_KEY_45
	{{"C",		"C",	(uchar)VK_C			},	{"C",		(uchar)VK_C			}},	// ID_KEY_46
	{{"V",		"V",	(uchar)VK_V			},	{"V",		(uchar)VK_V			}},	// ID_KEY_47
	{{"B",		"B",	(uchar)VK_B			},	{"B",		(uchar)VK_B			}},	// ID_KEY_48
	{{"N",		"N",	(uchar)VK_N			},	{"N",		(uchar)VK_N			}},	// ID_KEY_49
	{{"M",		"M",	(uchar)VK_M			},	{"M",		(uchar)VK_M			}},	// ID_KEY_50
	{{",",		"<",	(uchar)VK_OEM_COMMA	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_51
	{{".",		">",	(uchar)VK_OEM_PERIOD},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_52
	{{"\\",		"?",	(uchar)VK_OEM_2		},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_53
	{{"",		"",		(uchar)VK_NONE_00	},	{"",		(uchar)VK_NONE_00	}},	// ID_KEY_54
	//	{{"↑",		"↑",	(uchar)VK_UP		},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
	{{"\u2191",		"\u2192",	(uchar)VK_UP		},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
	{{"Shift",	"Shift",(uchar)VK_RSHIFT	},	{"Shift",	(uchar)VK_RSHIFT	}},	// ID_KEY_56

	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}},	// ID_KEY_57
	{{"Alt",	"Alt",	(uchar)VK_LMENU		},	{"Alt",		(uchar)VK_LMENU		}},	// ID_KEY_58
	{{"Win",	"Win",	(uchar)VK_LWIN		},	{"Win",		(uchar)VK_LWIN		}},	// ID_KEY_59
	{{"`",		"~",	(uchar)VK_OEM_7		},	{"`",		(uchar)VK_OEM_7		}},	// ID_KEY_60
	{{"Space",	"Space",(uchar)VK_SPACE		},	{"Space",	(uchar)VK_SPACE		}},	// ID_KEY_61
	{{"Ctrl",	"Ctrl",	(uchar)VK_RCONTROL	},	{"Ctrl",	(uchar)VK_RCONTROL	}},	// ID_KEY_62
	{{"Alt",	"Alt",	(uchar)VK_RMENU		},	{"Alt",		(uchar)VK_RMENU		}},	// ID_KEY_63
	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}},	// ID_KEY_64
	//	{{"←",		"←",	(uchar)VK_LEFT		},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	//	{{"↓",		"↓",	(uchar)VK_DOWN		},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	//	{{"→",		"→",	(uchar)VK_RIGHT		},	{"End",		(uchar)VK_END		}},	// ID_KEY_67
	{{"\u2190",	"\u2190",	(uchar)VK_LEFT		},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	{{"\u2193",	"\u2193",	(uchar)VK_DOWN		},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	{{"\u2192",	"\u2192",	(uchar)VK_RIGHT		},	{"End",		(uchar)VK_END		}}	// ID_KEY_67
  };

  // key top type
  KEYTOP_TYPE type;

  // keyboard size
  QSize keyboardSize;

  // shift key status
  bool onShiftKey;

  // control key status
  bool onControlKey;

  // alt key status
  bool onAltKey;

  // Fn key status
  bool onFnKey;

  // pushed shift key
  ID_KEY pushedShiftKey;

  // pushed control key
  ID_KEY pushedControlKey;

  // pushed alt key
  ID_KEY pushedAltKey;

  // pushed Fn key
  ID_KEY pushedFnKey;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_H
