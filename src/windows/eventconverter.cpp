// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// eventconverter.cpp

// Common Header
#include "common/common.h"

// Qt Header
#include <QString>

// Local Header
#include "eventconverter.h"

namespace qtbrynhildr {

// constructor
EventConverter::EventConverter()
  :
  shiftKeyControl(SHIFTKEY_THROUGH),
  outputLog(false)
{
}

// get shift key control
EventConverter::ShiftKeyControl EventConverter::getShiftKeyControl()
{
  return shiftKeyControl;
}

// for DEBUG

// name table of virtual keycode for windows
static const
QString nameTableOfVKCode[256] = {
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

// get name of virtual keycode
QString EventConverter::getVKCodeByString(uchar vkcode)
{
  return nameTableOfVKCode[vkcode];
}

} // end of namespace qtbrynhildr
