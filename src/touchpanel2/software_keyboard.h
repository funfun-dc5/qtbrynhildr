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

using namespace std;

namespace qtbrynhildr {

#ifdef USE_KEYLAYOUTFILE
class KeyLayoutFile;
#endif // USE_KEYLAYOUTFILE

// SoftwareKeyboard
class SoftwareKeyboard : public QWidget
{
#ifdef USE_KEYLAYOUTFILE
  friend class KeyLayoutFile;
#endif // USE_KEYLAYOUTFILE

public:
  // keyboard type
  typedef enum {
	KEYTOP_TYPE_JP,
	KEYTOP_TYPE_US,
	KEYTOP_TYPE_NATIVE,
	KEYTOP_TYPE_KLF,
	KEYTOP_TYPE_UNKNOWN
  } KEYTOP_TYPE;

#include "keytop.h"

public:
  // constructor
  SoftwareKeyboard(QWidget *parent = 0);
  SoftwareKeyboard(KEYTOP_TYPE type, QWidget *parent = 0);
#ifdef USE_KEYLAYOUTFILE
  SoftwareKeyboard(KeyLayoutFile *klf, QWidget *parent = 0);
#endif // USE_KEYLAYOUTFILE
  // destructor
  ~SoftwareKeyboard();

  // get keytop type
  KEYTOP_TYPE getKeytopType();

  // get keytop type name
  QString getKeytopTypeName();

  // set keytop type
  void setKeytopType(KEYTOP_TYPE type);

#ifdef USE_KEYLAYOUTFILE
  // set keytop type by key layout file
  void setKeytopType(KeyLayoutFile *klf);
#endif // USE_KEYLAYOUTFILE

  // reset size
  QSize resetSize();

protected:
  // paint event
  void paintEvent(QPaintEvent *event);

  // resize event
  void resizeEvent(QResizeEvent *event);

#if 0
  // minimum size hint
  QSize minimumSizeHint() const;
#endif

  // size hint
  QSize sizeHint() const;

  // mouse event
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  // key down
  virtual void keyDown(uchar key);
  
  // key up
  virtual void keyUp(uchar key);

private:
  // key id
  typedef enum {
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
	ID_KEY_68,
	ID_KEY_NUM,
	ID_KEY_0		= ID_KEY_NUM,
	ID_KEY_LSHIFT	= ID_KEY_43,
	ID_KEY_RSHIFT	= ID_KEY_56,
	ID_KEY_LCONTROL = ID_KEY_30,
	ID_KEY_RCONTROL = ID_KEY_62,
	ID_KEY_LALT		= ID_KEY_58,
	ID_KEY_RALT		= ID_KEY_63,
	ID_KEY_LFn		= ID_KEY_57,
	ID_KEY_RFn		= ID_KEY_64,
	ID_KEY_Fn		= ID_KEY_68
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
  static string getVKCodeByString(uchar vkcode);

#if QTB_DEBUG
  // print KeyTop
  static void printKeyTop(KeyTop *keyTop);
#endif // QTB_DEBUG

private:
  // original size for layout
  static const int WIDTH = 270;
  static const int HEIGHT = 135;

  // initial factor
  const qreal INITIAL_XFACTOR = 4.0;
#if defined(QTB_DEV_TOUCHPANEL)
  const qreal INITIAL_YFACTOR = 4.0*1.11; // for TEST Nexus 7 (1200/1080 = 1.11)
#else // defined(QTB_DEV_TOUCHPANEL)
  const qreal INITIAL_YFACTOR = 4.0;
#endif // defined(QTB_DEV_TOUCHPANEL)

  // layout table for keyboard
  const QRect keyLayout[ID_KEY_NUM] = {
	// 1st row
	QRect(  0, 60, 18, 15),
	QRect( 18, 60, 18, 15),
	QRect( 36, 60, 18, 15),
	QRect( 54, 60, 18, 15),
	QRect( 72, 60, 18, 15),
	QRect( 90, 60, 18, 15),
	QRect(108, 60, 18, 15),
	QRect(126, 60, 18, 15),
	QRect(144, 60, 18, 15),
	QRect(162, 60, 18, 15),
	QRect(180, 60, 18, 15),
	QRect(198, 60, 18, 15),
	QRect(216, 60, 18, 15),
	QRect(234, 60, 18, 15),
	QRect(252, 60, 18, 15),

	// 2nd row
	QRect(  0, 75, 24, 15),
	QRect( 24, 75, 18, 15),
	QRect( 42, 75, 18, 15),
	QRect( 60, 75, 18, 15),
	QRect( 78, 75, 18, 15),
	QRect( 96, 75, 18, 15),
	QRect(114, 75, 18, 15),
	QRect(132, 75, 18, 15),
	QRect(150, 75, 18, 15),
	QRect(168, 75, 18, 15),
	QRect(186, 75, 18, 15),
	QRect(204, 75, 18, 15),
	QRect(222, 75, 26, 15),
	QRect(248, 75, 22, 30),

	// 3rd row
	QRect(  0, 90, 32, 15),
	QRect( 32, 90, 18, 15),
	QRect( 50, 90, 18, 15),
	QRect( 68, 90, 18, 15),
	QRect( 86, 90, 18, 15),
	QRect(104, 90, 18, 15),
	QRect(122, 90, 18, 15),
	QRect(140, 90, 18, 15),
	QRect(158, 90, 18, 15),
	QRect(176, 90, 18, 15),
	QRect(194, 90, 18, 15),
	QRect(212, 90, 18, 15),
	QRect(230, 90, 18, 15),

	// 4th row
	QRect(  0,105, 36, 15),
	QRect( 36,105, 18, 15),
	QRect( 54,105, 18, 15),
	QRect( 72,105, 18, 15),
	QRect( 90,105, 18, 15),
	QRect(108,105, 18, 15),
	QRect(126,105, 18, 15),
	QRect(144,105, 18, 15),
	QRect(162,105, 18, 15),
	QRect(180,105, 18, 15),
	QRect(198,105, 18, 15),
	QRect(216,105, 18, 15),
	QRect(234,105, 18, 15),
	QRect(252,105, 18, 15),

	// 5th row
	QRect(  0,120, 36, 15),
	QRect( 36,120, 18, 15),
	QRect( 54,120, 18, 15),
	QRect( 72,120, 18, 15),
	QRect( 90,120, 72, 15),
	QRect(162,120, 18, 15),
	QRect(180,120, 18, 15),
	QRect(198,120, 18, 15),
	QRect(216,120, 18, 15),
	QRect(234,120, 18, 15),
	QRect(252,120, 18, 15),

	// Fn button
	QRect(  0,  0, 18, 15)
};

  // layout table for keyboard
  typedef struct {
	QRect rect;
	bool pushed;
  } Layout;
  Layout layout[ID_KEY_NUM];

  // key top table
  const KeyTop *keyTopTable;

#if 0 // for TEST
  // temporary key top table
  KeyTop *tempKeyTopTable;
#endif // for TEST

  // key top table for JP
  const KeyTop keyTopTable_JP[ID_KEY_NUM] = {
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
	{{"\uffe5",	"|",	(uchar)VK_OEM_5		},	{"Ins",		(uchar)VK_INSERT	}},	// ID_KEY_14
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
	{{"\u2191",		"\u2191",	(uchar)VK_UP},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
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
	{{"\u2190",	"\u2190",	(uchar)VK_LEFT	},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	{{"\u2193",	"\u2193",	(uchar)VK_DOWN	},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	{{"\u2192",	"\u2192",	(uchar)VK_RIGHT	},	{"End",		(uchar)VK_END		}},	// ID_KEY_67

	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}}	// ID_KEY_68
  };

  // key top table for US
  const KeyTop keyTopTable_US[ID_KEY_NUM] = {
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
	{{"\u2191",		"\u2191",	(uchar)VK_UP},	{"PgU",		(uchar)VK_PRIOR		}},	// ID_KEY_55
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
	{{"\u2190",	"\u2190",	(uchar)VK_LEFT	},	{"Home",	(uchar)VK_HOME		}},	// ID_KEY_65
	{{"\u2193",	"\u2193",	(uchar)VK_DOWN	},	{"PgD",		(uchar)VK_NEXT		}},	// ID_KEY_66
	{{"\u2192",	"\u2192",	(uchar)VK_RIGHT	},	{"End",		(uchar)VK_END		}},	// ID_KEY_67

	{{"Fn",		"Fn",	(uchar)VK_NONE_00	},	{"Fn",		(uchar)VK_NONE_00	}}	// ID_KEY_68
  };

#ifdef USE_KEYLAYOUTFILE
  // key layout file
  KeyLayoutFile *klf;
#endif // USE_KEYLAYOUTFILE

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
