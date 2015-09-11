// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard.cpp

// Common Header
#include "common/common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_keyboard.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

// for DEBUG

// name table of virtual keycode for windows
static const
string nameTableOfVKCode[256] = {
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

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboard::SoftwareKeyboard(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent)
  :
  QWidget(parent),
  settings(settings),
  keyBuffer(keyBuffer),
  onShiftKey(false),
  onControlKey(false),
  onAltKey(false),
  onFnKey(false),
  // for DEBUG
  outputLog(true)
{
  setupUi(this);

  // connect signals
  // pressed signals
  connect(pushButton_1, SIGNAL(pressed()), this, SLOT(pressedKey_1()));
  connect(pushButton_2, SIGNAL(pressed()), this, SLOT(pressedKey_2()));
  connect(pushButton_3, SIGNAL(pressed()), this, SLOT(pressedKey_3()));
  connect(pushButton_4, SIGNAL(pressed()), this, SLOT(pressedKey_4()));
  connect(pushButton_5, SIGNAL(pressed()), this, SLOT(pressedKey_5()));
  connect(pushButton_6, SIGNAL(pressed()), this, SLOT(pressedKey_6()));
  connect(pushButton_7, SIGNAL(pressed()), this, SLOT(pressedKey_7()));
  connect(pushButton_8, SIGNAL(pressed()), this, SLOT(pressedKey_8()));
  connect(pushButton_9, SIGNAL(pressed()), this, SLOT(pressedKey_9()));
  connect(pushButton_10, SIGNAL(pressed()), this, SLOT(pressedKey_10()));
  connect(pushButton_11, SIGNAL(pressed()), this, SLOT(pressedKey_11()));
  connect(pushButton_12, SIGNAL(pressed()), this, SLOT(pressedKey_12()));
  connect(pushButton_13, SIGNAL(pressed()), this, SLOT(pressedKey_13()));
  connect(pushButton_14, SIGNAL(pressed()), this, SLOT(pressedKey_14()));
  connect(pushButton_15, SIGNAL(pressed()), this, SLOT(pressedKey_15()));
  connect(pushButton_16, SIGNAL(pressed()), this, SLOT(pressedKey_16()));
  connect(pushButton_17, SIGNAL(pressed()), this, SLOT(pressedKey_17()));
  connect(pushButton_18, SIGNAL(pressed()), this, SLOT(pressedKey_18()));
  connect(pushButton_19, SIGNAL(pressed()), this, SLOT(pressedKey_19()));
  connect(pushButton_20, SIGNAL(pressed()), this, SLOT(pressedKey_20()));
  connect(pushButton_21, SIGNAL(pressed()), this, SLOT(pressedKey_21()));
  connect(pushButton_22, SIGNAL(pressed()), this, SLOT(pressedKey_22()));
  connect(pushButton_23, SIGNAL(pressed()), this, SLOT(pressedKey_23()));
  connect(pushButton_24, SIGNAL(pressed()), this, SLOT(pressedKey_24()));
  connect(pushButton_25, SIGNAL(pressed()), this, SLOT(pressedKey_25()));
  connect(pushButton_26, SIGNAL(pressed()), this, SLOT(pressedKey_26()));
  connect(pushButton_27, SIGNAL(pressed()), this, SLOT(pressedKey_27()));
  connect(pushButton_28, SIGNAL(pressed()), this, SLOT(pressedKey_28()));
  connect(pushButton_29, SIGNAL(pressed()), this, SLOT(pressedKey_29()));
  connect(pushButton_30, SIGNAL(pressed()), this, SLOT(pressedKey_30()));
  connect(pushButton_31, SIGNAL(pressed()), this, SLOT(pressedKey_31()));
  connect(pushButton_32, SIGNAL(pressed()), this, SLOT(pressedKey_32()));
  connect(pushButton_33, SIGNAL(pressed()), this, SLOT(pressedKey_33()));
  connect(pushButton_34, SIGNAL(pressed()), this, SLOT(pressedKey_34()));
  connect(pushButton_35, SIGNAL(pressed()), this, SLOT(pressedKey_35()));
  connect(pushButton_36, SIGNAL(pressed()), this, SLOT(pressedKey_36()));
  connect(pushButton_37, SIGNAL(pressed()), this, SLOT(pressedKey_37()));
  connect(pushButton_38, SIGNAL(pressed()), this, SLOT(pressedKey_38()));
  connect(pushButton_39, SIGNAL(pressed()), this, SLOT(pressedKey_39()));
  connect(pushButton_40, SIGNAL(pressed()), this, SLOT(pressedKey_40()));
  connect(pushButton_41, SIGNAL(pressed()), this, SLOT(pressedKey_41()));
  connect(pushButton_42, SIGNAL(pressed()), this, SLOT(pressedKey_42()));
  connect(pushButton_43, SIGNAL(pressed()), this, SLOT(pressedKey_43()));
  connect(pushButton_44, SIGNAL(pressed()), this, SLOT(pressedKey_44()));
  connect(pushButton_45, SIGNAL(pressed()), this, SLOT(pressedKey_45()));
  connect(pushButton_46, SIGNAL(pressed()), this, SLOT(pressedKey_46()));
  connect(pushButton_47, SIGNAL(pressed()), this, SLOT(pressedKey_47()));
  connect(pushButton_48, SIGNAL(pressed()), this, SLOT(pressedKey_48()));
  connect(pushButton_49, SIGNAL(pressed()), this, SLOT(pressedKey_49()));
  connect(pushButton_50, SIGNAL(pressed()), this, SLOT(pressedKey_50()));
  connect(pushButton_51, SIGNAL(pressed()), this, SLOT(pressedKey_51()));
  connect(pushButton_52, SIGNAL(pressed()), this, SLOT(pressedKey_52()));
  connect(pushButton_53, SIGNAL(pressed()), this, SLOT(pressedKey_53()));
  connect(pushButton_54, SIGNAL(pressed()), this, SLOT(pressedKey_54()));
  connect(pushButton_55, SIGNAL(pressed()), this, SLOT(pressedKey_55()));
  connect(pushButton_56, SIGNAL(pressed()), this, SLOT(pressedKey_56()));
  connect(pushButton_57, SIGNAL(pressed()), this, SLOT(pressedKey_57()));
  connect(pushButton_58, SIGNAL(pressed()), this, SLOT(pressedKey_58()));
  connect(pushButton_59, SIGNAL(pressed()), this, SLOT(pressedKey_59()));
  connect(pushButton_60, SIGNAL(pressed()), this, SLOT(pressedKey_60()));
  connect(pushButton_61, SIGNAL(pressed()), this, SLOT(pressedKey_61()));
  connect(pushButton_62, SIGNAL(pressed()), this, SLOT(pressedKey_62()));
  connect(pushButton_63, SIGNAL(pressed()), this, SLOT(pressedKey_63()));
  connect(pushButton_64, SIGNAL(pressed()), this, SLOT(pressedKey_64()));
  connect(pushButton_65, SIGNAL(pressed()), this, SLOT(pressedKey_65()));
  connect(pushButton_66, SIGNAL(pressed()), this, SLOT(pressedKey_66()));
  connect(pushButton_67, SIGNAL(pressed()), this, SLOT(pressedKey_67()));

  // released signals
  connect(pushButton_1, SIGNAL(released()), this, SLOT(releasedKey_1()));
  connect(pushButton_2, SIGNAL(released()), this, SLOT(releasedKey_2()));
  connect(pushButton_3, SIGNAL(released()), this, SLOT(releasedKey_3()));
  connect(pushButton_4, SIGNAL(released()), this, SLOT(releasedKey_4()));
  connect(pushButton_5, SIGNAL(released()), this, SLOT(releasedKey_5()));
  connect(pushButton_6, SIGNAL(released()), this, SLOT(releasedKey_6()));
  connect(pushButton_7, SIGNAL(released()), this, SLOT(releasedKey_7()));
  connect(pushButton_8, SIGNAL(released()), this, SLOT(releasedKey_8()));
  connect(pushButton_9, SIGNAL(released()), this, SLOT(releasedKey_9()));
  connect(pushButton_10, SIGNAL(released()), this, SLOT(releasedKey_10()));
  connect(pushButton_11, SIGNAL(released()), this, SLOT(releasedKey_11()));
  connect(pushButton_12, SIGNAL(released()), this, SLOT(releasedKey_12()));
  connect(pushButton_13, SIGNAL(released()), this, SLOT(releasedKey_13()));
  connect(pushButton_14, SIGNAL(released()), this, SLOT(releasedKey_14()));
  connect(pushButton_15, SIGNAL(released()), this, SLOT(releasedKey_15()));
  connect(pushButton_16, SIGNAL(released()), this, SLOT(releasedKey_16()));
  connect(pushButton_17, SIGNAL(released()), this, SLOT(releasedKey_17()));
  connect(pushButton_18, SIGNAL(released()), this, SLOT(releasedKey_18()));
  connect(pushButton_19, SIGNAL(released()), this, SLOT(releasedKey_19()));
  connect(pushButton_20, SIGNAL(released()), this, SLOT(releasedKey_20()));
  connect(pushButton_21, SIGNAL(released()), this, SLOT(releasedKey_21()));
  connect(pushButton_22, SIGNAL(released()), this, SLOT(releasedKey_22()));
  connect(pushButton_23, SIGNAL(released()), this, SLOT(releasedKey_23()));
  connect(pushButton_24, SIGNAL(released()), this, SLOT(releasedKey_24()));
  connect(pushButton_25, SIGNAL(released()), this, SLOT(releasedKey_25()));
  connect(pushButton_26, SIGNAL(released()), this, SLOT(releasedKey_26()));
  connect(pushButton_27, SIGNAL(released()), this, SLOT(releasedKey_27()));
  connect(pushButton_28, SIGNAL(released()), this, SLOT(releasedKey_28()));
  connect(pushButton_29, SIGNAL(released()), this, SLOT(releasedKey_29()));
  connect(pushButton_30, SIGNAL(released()), this, SLOT(releasedKey_30()));
  connect(pushButton_31, SIGNAL(released()), this, SLOT(releasedKey_31()));
  connect(pushButton_32, SIGNAL(released()), this, SLOT(releasedKey_32()));
  connect(pushButton_33, SIGNAL(released()), this, SLOT(releasedKey_33()));
  connect(pushButton_34, SIGNAL(released()), this, SLOT(releasedKey_34()));
  connect(pushButton_35, SIGNAL(released()), this, SLOT(releasedKey_35()));
  connect(pushButton_36, SIGNAL(released()), this, SLOT(releasedKey_36()));
  connect(pushButton_37, SIGNAL(released()), this, SLOT(releasedKey_37()));
  connect(pushButton_38, SIGNAL(released()), this, SLOT(releasedKey_38()));
  connect(pushButton_39, SIGNAL(released()), this, SLOT(releasedKey_39()));
  connect(pushButton_40, SIGNAL(released()), this, SLOT(releasedKey_40()));
  connect(pushButton_41, SIGNAL(released()), this, SLOT(releasedKey_41()));
  connect(pushButton_42, SIGNAL(released()), this, SLOT(releasedKey_42()));
  connect(pushButton_43, SIGNAL(released()), this, SLOT(releasedKey_43()));
  connect(pushButton_44, SIGNAL(released()), this, SLOT(releasedKey_44()));
  connect(pushButton_45, SIGNAL(released()), this, SLOT(releasedKey_45()));
  connect(pushButton_46, SIGNAL(released()), this, SLOT(releasedKey_46()));
  connect(pushButton_47, SIGNAL(released()), this, SLOT(releasedKey_47()));
  connect(pushButton_48, SIGNAL(released()), this, SLOT(releasedKey_48()));
  connect(pushButton_49, SIGNAL(released()), this, SLOT(releasedKey_49()));
  connect(pushButton_50, SIGNAL(released()), this, SLOT(releasedKey_50()));
  connect(pushButton_51, SIGNAL(released()), this, SLOT(releasedKey_51()));
  connect(pushButton_52, SIGNAL(released()), this, SLOT(releasedKey_52()));
  connect(pushButton_53, SIGNAL(released()), this, SLOT(releasedKey_53()));
  connect(pushButton_54, SIGNAL(released()), this, SLOT(releasedKey_54()));
  connect(pushButton_55, SIGNAL(released()), this, SLOT(releasedKey_55()));
  connect(pushButton_56, SIGNAL(released()), this, SLOT(releasedKey_56()));
  connect(pushButton_57, SIGNAL(released()), this, SLOT(releasedKey_57()));
  connect(pushButton_58, SIGNAL(released()), this, SLOT(releasedKey_58()));
  connect(pushButton_59, SIGNAL(released()), this, SLOT(releasedKey_59()));
  connect(pushButton_60, SIGNAL(released()), this, SLOT(releasedKey_60()));
  connect(pushButton_61, SIGNAL(released()), this, SLOT(releasedKey_61()));
  connect(pushButton_62, SIGNAL(released()), this, SLOT(releasedKey_62()));
  connect(pushButton_63, SIGNAL(released()), this, SLOT(releasedKey_63()));
  connect(pushButton_64, SIGNAL(released()), this, SLOT(releasedKey_64()));
  connect(pushButton_65, SIGNAL(released()), this, SLOT(releasedKey_65()));
  connect(pushButton_66, SIGNAL(released()), this, SLOT(releasedKey_66()));
  connect(pushButton_67, SIGNAL(released()), this, SLOT(releasedKey_67()));

  // set button table
  buttons[0] = 0; // DUMMY
  buttons[ID_KEY_1] = pushButton_1;
  buttons[ID_KEY_2] = pushButton_2;
  buttons[ID_KEY_3] = pushButton_3;
  buttons[ID_KEY_4] = pushButton_4;
  buttons[ID_KEY_5] = pushButton_5;
  buttons[ID_KEY_6] = pushButton_6;
  buttons[ID_KEY_7] = pushButton_7;
  buttons[ID_KEY_8] = pushButton_8;
  buttons[ID_KEY_9] = pushButton_9;
  buttons[ID_KEY_10] = pushButton_10;
  buttons[ID_KEY_11] = pushButton_11;
  buttons[ID_KEY_12] = pushButton_12;
  buttons[ID_KEY_13] = pushButton_13;
  buttons[ID_KEY_14] = pushButton_14;
  buttons[ID_KEY_15] = pushButton_15;
  buttons[ID_KEY_16] = pushButton_16;
  buttons[ID_KEY_17] = pushButton_17;
  buttons[ID_KEY_18] = pushButton_18;
  buttons[ID_KEY_19] = pushButton_19;
  buttons[ID_KEY_20] = pushButton_20;
  buttons[ID_KEY_21] = pushButton_21;
  buttons[ID_KEY_22] = pushButton_22;
  buttons[ID_KEY_23] = pushButton_23;
  buttons[ID_KEY_24] = pushButton_24;
  buttons[ID_KEY_25] = pushButton_25;
  buttons[ID_KEY_26] = pushButton_26;
  buttons[ID_KEY_27] = pushButton_27;
  buttons[ID_KEY_28] = pushButton_28;
  buttons[ID_KEY_29] = pushButton_29;
  buttons[ID_KEY_30] = pushButton_30;
  buttons[ID_KEY_31] = pushButton_31;
  buttons[ID_KEY_32] = pushButton_32;
  buttons[ID_KEY_33] = pushButton_33;
  buttons[ID_KEY_34] = pushButton_34;
  buttons[ID_KEY_35] = pushButton_35;
  buttons[ID_KEY_36] = pushButton_36;
  buttons[ID_KEY_37] = pushButton_37;
  buttons[ID_KEY_38] = pushButton_38;
  buttons[ID_KEY_39] = pushButton_39;
  buttons[ID_KEY_40] = pushButton_40;
  buttons[ID_KEY_41] = pushButton_41;
  buttons[ID_KEY_42] = pushButton_42;
  buttons[ID_KEY_43] = pushButton_43;
  buttons[ID_KEY_44] = pushButton_44;
  buttons[ID_KEY_45] = pushButton_45;
  buttons[ID_KEY_46] = pushButton_46;
  buttons[ID_KEY_47] = pushButton_47;
  buttons[ID_KEY_48] = pushButton_48;
  buttons[ID_KEY_49] = pushButton_49;
  buttons[ID_KEY_50] = pushButton_50;
  buttons[ID_KEY_51] = pushButton_51;
  buttons[ID_KEY_52] = pushButton_52;
  buttons[ID_KEY_53] = pushButton_53;
  buttons[ID_KEY_54] = pushButton_54;
  buttons[ID_KEY_55] = pushButton_55;
  buttons[ID_KEY_56] = pushButton_56;
  buttons[ID_KEY_57] = pushButton_57;
  buttons[ID_KEY_58] = pushButton_58;
  buttons[ID_KEY_59] = pushButton_59;
  buttons[ID_KEY_60] = pushButton_60;
  buttons[ID_KEY_61] = pushButton_61;
  buttons[ID_KEY_62] = pushButton_62;
  buttons[ID_KEY_63] = pushButton_63;
  buttons[ID_KEY_64] = pushButton_64;
  buttons[ID_KEY_65] = pushButton_65;
  buttons[ID_KEY_66] = pushButton_66;
  buttons[ID_KEY_67] = pushButton_67;

  // update size
  updateSize();
}

// size hint
QSize SoftwareKeyboard::sizeHint() const
{
  return size();
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// get name of virtual keycode
string SoftwareKeyboard::getVKCodeByString(uchar vkcode)
{
  return nameTableOfVKCode[vkcode];
}

// update size
void SoftwareKeyboard::updateSize()
{
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
// pressed slots
void SoftwareKeyboard::pressedKey_1()
{
  pressedKey(ID_KEY_1);
}

void SoftwareKeyboard::pressedKey_2()
{
  pressedKey(ID_KEY_2);
}

void SoftwareKeyboard::pressedKey_3()
{
  pressedKey(ID_KEY_3);
}

void SoftwareKeyboard::pressedKey_4()
{
  pressedKey(ID_KEY_4);
}

void SoftwareKeyboard::pressedKey_5()
{
  pressedKey(ID_KEY_5);
}

void SoftwareKeyboard::pressedKey_6()
{
  pressedKey(ID_KEY_6);
}

void SoftwareKeyboard::pressedKey_7()
{
  pressedKey(ID_KEY_7);
}

void SoftwareKeyboard::pressedKey_8()
{
  pressedKey(ID_KEY_8);
}

void SoftwareKeyboard::pressedKey_9()
{
  pressedKey(ID_KEY_9);
}

void SoftwareKeyboard::pressedKey_10()
{
  pressedKey(ID_KEY_10);
}

void SoftwareKeyboard::pressedKey_11()
{
  pressedKey(ID_KEY_11);
}

void SoftwareKeyboard::pressedKey_12()
{
  pressedKey(ID_KEY_12);
}

void SoftwareKeyboard::pressedKey_13()
{
  pressedKey(ID_KEY_13);
}

void SoftwareKeyboard::pressedKey_14()
{
  pressedKey(ID_KEY_14);
}

void SoftwareKeyboard::pressedKey_15()
{
  pressedKey(ID_KEY_15);
}

void SoftwareKeyboard::pressedKey_16()
{
  pressedKey(ID_KEY_16);
}

void SoftwareKeyboard::pressedKey_17()
{
  pressedKey(ID_KEY_17);
}

void SoftwareKeyboard::pressedKey_18()
{
  pressedKey(ID_KEY_18);
}

void SoftwareKeyboard::pressedKey_19()
{
  pressedKey(ID_KEY_19);
}

void SoftwareKeyboard::pressedKey_20()
{
  pressedKey(ID_KEY_20);
}

void SoftwareKeyboard::pressedKey_21()
{
  pressedKey(ID_KEY_21);
}

void SoftwareKeyboard::pressedKey_22()
{
  pressedKey(ID_KEY_22);
}

void SoftwareKeyboard::pressedKey_23()
{
  pressedKey(ID_KEY_23);
}

void SoftwareKeyboard::pressedKey_24()
{
  pressedKey(ID_KEY_24);
}

void SoftwareKeyboard::pressedKey_25()
{
  pressedKey(ID_KEY_25);
}

void SoftwareKeyboard::pressedKey_26()
{
  pressedKey(ID_KEY_26);
}

void SoftwareKeyboard::pressedKey_27()
{
  pressedKey(ID_KEY_27);
}

void SoftwareKeyboard::pressedKey_28()
{
  pressedKey(ID_KEY_28);
}

void SoftwareKeyboard::pressedKey_29()
{
  pressedKey(ID_KEY_29);
}

void SoftwareKeyboard::pressedKey_30()
{
  pressedKey(ID_KEY_30);
}

void SoftwareKeyboard::pressedKey_31()
{
  pressedKey(ID_KEY_31);
}

void SoftwareKeyboard::pressedKey_32()
{
  pressedKey(ID_KEY_32);
}

void SoftwareKeyboard::pressedKey_33()
{
  pressedKey(ID_KEY_33);
}

void SoftwareKeyboard::pressedKey_34()
{
  pressedKey(ID_KEY_34);
}

void SoftwareKeyboard::pressedKey_35()
{
  pressedKey(ID_KEY_35);
}

void SoftwareKeyboard::pressedKey_36()
{
  pressedKey(ID_KEY_36);
}

void SoftwareKeyboard::pressedKey_37()
{
  pressedKey(ID_KEY_37);
}

void SoftwareKeyboard::pressedKey_38()
{
  pressedKey(ID_KEY_38);
}

void SoftwareKeyboard::pressedKey_39()
{
  pressedKey(ID_KEY_39);
}

void SoftwareKeyboard::pressedKey_40()
{
  pressedKey(ID_KEY_40);
}

void SoftwareKeyboard::pressedKey_41()
{
  pressedKey(ID_KEY_41);
}

void SoftwareKeyboard::pressedKey_42()
{
  pressedKey(ID_KEY_42);
}

void SoftwareKeyboard::pressedKey_43()
{
  pressedKey(ID_KEY_43);
}

void SoftwareKeyboard::pressedKey_44()
{
  pressedKey(ID_KEY_44);
}

void SoftwareKeyboard::pressedKey_45()
{
  pressedKey(ID_KEY_45);
}

void SoftwareKeyboard::pressedKey_46()
{
  pressedKey(ID_KEY_46);
}

void SoftwareKeyboard::pressedKey_47()
{
  pressedKey(ID_KEY_47);
}

void SoftwareKeyboard::pressedKey_48()
{
  pressedKey(ID_KEY_48);
}

void SoftwareKeyboard::pressedKey_49()
{
  pressedKey(ID_KEY_49);
}

void SoftwareKeyboard::pressedKey_50()
{
  pressedKey(ID_KEY_50);
}

void SoftwareKeyboard::pressedKey_51()
{
  pressedKey(ID_KEY_51);
}

void SoftwareKeyboard::pressedKey_52()
{
  pressedKey(ID_KEY_52);
}

void SoftwareKeyboard::pressedKey_53()
{
  pressedKey(ID_KEY_53);
}

void SoftwareKeyboard::pressedKey_54()
{
  pressedKey(ID_KEY_54);
}

void SoftwareKeyboard::pressedKey_55()
{
  pressedKey(ID_KEY_55);
}

void SoftwareKeyboard::pressedKey_56()
{
  pressedKey(ID_KEY_56);
}

void SoftwareKeyboard::pressedKey_57()
{
  pressedKey(ID_KEY_57);
}

void SoftwareKeyboard::pressedKey_58()
{
  pressedKey(ID_KEY_58);
}

void SoftwareKeyboard::pressedKey_59()
{
  pressedKey(ID_KEY_59);
}

void SoftwareKeyboard::pressedKey_60()
{
  pressedKey(ID_KEY_60);
}

void SoftwareKeyboard::pressedKey_61()
{
  pressedKey(ID_KEY_61);
}

void SoftwareKeyboard::pressedKey_62()
{
  pressedKey(ID_KEY_62);
}

void SoftwareKeyboard::pressedKey_63()
{
  pressedKey(ID_KEY_63);
}

void SoftwareKeyboard::pressedKey_64()
{
  pressedKey(ID_KEY_64);
}

void SoftwareKeyboard::pressedKey_65()
{
  pressedKey(ID_KEY_65);
}

void SoftwareKeyboard::pressedKey_66()
{
  pressedKey(ID_KEY_66);
}

void SoftwareKeyboard::pressedKey_67()
{
  pressedKey(ID_KEY_67);
}

// released slots
void SoftwareKeyboard::releasedKey_1()
{
  releasedKey(ID_KEY_1);
}

void SoftwareKeyboard::releasedKey_2()
{
  releasedKey(ID_KEY_2);
}

void SoftwareKeyboard::releasedKey_3()
{
  releasedKey(ID_KEY_3);
}

void SoftwareKeyboard::releasedKey_4()
{
  releasedKey(ID_KEY_4);
}

void SoftwareKeyboard::releasedKey_5()
{
  releasedKey(ID_KEY_5);
}

void SoftwareKeyboard::releasedKey_6()
{
  releasedKey(ID_KEY_6);
}

void SoftwareKeyboard::releasedKey_7()
{
  releasedKey(ID_KEY_7);
}

void SoftwareKeyboard::releasedKey_8()
{
  releasedKey(ID_KEY_8);
}

void SoftwareKeyboard::releasedKey_9()
{
  releasedKey(ID_KEY_9);
}

void SoftwareKeyboard::releasedKey_10()
{
  releasedKey(ID_KEY_10);
}

void SoftwareKeyboard::releasedKey_11()
{
  releasedKey(ID_KEY_11);
}

void SoftwareKeyboard::releasedKey_12()
{
  releasedKey(ID_KEY_12);
}

void SoftwareKeyboard::releasedKey_13()
{
  releasedKey(ID_KEY_13);
}

void SoftwareKeyboard::releasedKey_14()
{
  releasedKey(ID_KEY_14);
}

void SoftwareKeyboard::releasedKey_15()
{
  releasedKey(ID_KEY_15);
}

void SoftwareKeyboard::releasedKey_16()
{
  releasedKey(ID_KEY_16);
}

void SoftwareKeyboard::releasedKey_17()
{
  releasedKey(ID_KEY_17);
}

void SoftwareKeyboard::releasedKey_18()
{
  releasedKey(ID_KEY_18);
}

void SoftwareKeyboard::releasedKey_19()
{
  releasedKey(ID_KEY_19);
}

void SoftwareKeyboard::releasedKey_20()
{
  releasedKey(ID_KEY_20);
}

void SoftwareKeyboard::releasedKey_21()
{
  releasedKey(ID_KEY_21);
}

void SoftwareKeyboard::releasedKey_22()
{
  releasedKey(ID_KEY_22);
}

void SoftwareKeyboard::releasedKey_23()
{
  releasedKey(ID_KEY_23);
}

void SoftwareKeyboard::releasedKey_24()
{
  releasedKey(ID_KEY_24);
}

void SoftwareKeyboard::releasedKey_25()
{
  releasedKey(ID_KEY_25);
}

void SoftwareKeyboard::releasedKey_26()
{
  releasedKey(ID_KEY_26);
}

void SoftwareKeyboard::releasedKey_27()
{
  releasedKey(ID_KEY_27);
}

void SoftwareKeyboard::releasedKey_28()
{
  releasedKey(ID_KEY_28);
}

void SoftwareKeyboard::releasedKey_29()
{
  releasedKey(ID_KEY_29);
}

void SoftwareKeyboard::releasedKey_30()
{
  releasedKey(ID_KEY_30);
}

void SoftwareKeyboard::releasedKey_31()
{
  releasedKey(ID_KEY_31);
}

void SoftwareKeyboard::releasedKey_32()
{
  releasedKey(ID_KEY_32);
}

void SoftwareKeyboard::releasedKey_33()
{
  releasedKey(ID_KEY_33);
}

void SoftwareKeyboard::releasedKey_34()
{
  releasedKey(ID_KEY_34);
}

void SoftwareKeyboard::releasedKey_35()
{
  releasedKey(ID_KEY_35);
}

void SoftwareKeyboard::releasedKey_36()
{
  releasedKey(ID_KEY_36);
}

void SoftwareKeyboard::releasedKey_37()
{
  releasedKey(ID_KEY_37);
}

void SoftwareKeyboard::releasedKey_38()
{
  releasedKey(ID_KEY_38);
}

void SoftwareKeyboard::releasedKey_39()
{
  releasedKey(ID_KEY_39);
}

void SoftwareKeyboard::releasedKey_40()
{
  releasedKey(ID_KEY_40);
}

void SoftwareKeyboard::releasedKey_41()
{
  releasedKey(ID_KEY_41);
}

void SoftwareKeyboard::releasedKey_42()
{
  releasedKey(ID_KEY_42);
}

void SoftwareKeyboard::releasedKey_43()
{
  releasedKey(ID_KEY_43);
}

void SoftwareKeyboard::releasedKey_44()
{
  releasedKey(ID_KEY_44);
}

void SoftwareKeyboard::releasedKey_45()
{
  releasedKey(ID_KEY_45);
}

void SoftwareKeyboard::releasedKey_46()
{
  releasedKey(ID_KEY_46);
}

void SoftwareKeyboard::releasedKey_47()
{
  releasedKey(ID_KEY_47);
}

void SoftwareKeyboard::releasedKey_48()
{
  releasedKey(ID_KEY_48);
}

void SoftwareKeyboard::releasedKey_49()
{
  releasedKey(ID_KEY_49);
}

void SoftwareKeyboard::releasedKey_50()
{
  releasedKey(ID_KEY_50);
}

void SoftwareKeyboard::releasedKey_51()
{
  releasedKey(ID_KEY_51);
}

void SoftwareKeyboard::releasedKey_52()
{
  releasedKey(ID_KEY_52);
}

void SoftwareKeyboard::releasedKey_53()
{
  releasedKey(ID_KEY_53);
}

void SoftwareKeyboard::releasedKey_54()
{
  releasedKey(ID_KEY_54);
}

void SoftwareKeyboard::releasedKey_55()
{
  releasedKey(ID_KEY_55);
}

void SoftwareKeyboard::releasedKey_56()
{
  releasedKey(ID_KEY_56);
}

void SoftwareKeyboard::releasedKey_57()
{
  releasedKey(ID_KEY_57);
}

void SoftwareKeyboard::releasedKey_58()
{
  releasedKey(ID_KEY_58);
}

void SoftwareKeyboard::releasedKey_59()
{
  releasedKey(ID_KEY_59);
}

void SoftwareKeyboard::releasedKey_60()
{
  releasedKey(ID_KEY_60);
}

void SoftwareKeyboard::releasedKey_61()
{
  releasedKey(ID_KEY_61);
}

void SoftwareKeyboard::releasedKey_62()
{
  releasedKey(ID_KEY_62);
}

void SoftwareKeyboard::releasedKey_63()
{
  releasedKey(ID_KEY_63);
}

void SoftwareKeyboard::releasedKey_64()
{
  releasedKey(ID_KEY_64);
}

void SoftwareKeyboard::releasedKey_65()
{
  releasedKey(ID_KEY_65);
}

void SoftwareKeyboard::releasedKey_66()
{
  releasedKey(ID_KEY_66);
}

void SoftwareKeyboard::releasedKey_67()
{
  releasedKey(ID_KEY_67);
}

} // end of namespace qtbrynhildr
