// -*- mode: c; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>
//
%{
#ifdef _MSC_VER
#include "common/msvc.h"
#endif // _MSC_VER

#include <stdio.h>
#include <string.h>

#include "klfcompiler.h"

#include "keylayoutfile.h"
#include "touchpanel2/keytop.h"
#include "windows/keyevent.h"

extern int yylex();
void yyerror(char *msg);

int getENVVAR_ID(const char *name);
int getVK_ID(const char *name);
Key getKey_ID(const char *name);
int getID_Key(Key key);

// platform
const int this_platform = THIS_PLATFORM;

// ENVVAR
#define ID_ENVVAR_NAME			0
#define ID_ENVVAR_AUTHOR		1
#define ID_ENVVAR_SPECVERSION	2
#define ID_ENVVAR_NUM			3
const char *envvarname[ID_ENVVAR_NUM] = {
  "Name",
  "Author",
  "SpecVersion"
};

// VK_ID
const char *vkname[256] = {
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
  "VK_KANA",
  "VK_NONE_16",
  "VK_JUNJA",
  "VK_FINAL",
  "VK_KANJI",
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

typedef struct {
	const char *name;
	Key key;
} KeyEntry;

#define KEY_ENTRY_NUM 439

KeyEntry keys[KEY_ENTRY_NUM] = {
  {"Key_Escape", Key_Escape},
  {"Key_Tab", Key_Tab},
  {"Key_Backtab", Key_Backtab},
  {"Key_Backspace", Key_Backspace},
  {"Key_Return", Key_Return},
  {"Key_Enter", Key_Enter},
  {"Key_Insert", Key_Insert},
  {"Key_Delete", Key_Delete},
  {"Key_Pause", Key_Pause},
  {"Key_Print", Key_Print},
  {"Key_SysReq", Key_SysReq},
  {"Key_Clear", Key_Clear},
  {"Key_Home", Key_Home},
  {"Key_End", Key_End},
  {"Key_Left", Key_Left},
  {"Key_Up", Key_Up},
  {"Key_Right", Key_Right},
  {"Key_Down", Key_Down},
  {"Key_PageUp", Key_PageUp},
  {"Key_PageDown", Key_PageDown},
  {"Key_Shift", Key_Shift},
  {"Key_Control", Key_Control},
  {"Key_Meta", Key_Meta},
  {"Key_Alt", Key_Alt},
  {"Key_AltGr", Key_AltGr},
  {"Key_CapsLock", Key_CapsLock},
  {"Key_NumLock", Key_NumLock},
  {"Key_ScrollLock", Key_ScrollLock},
  {"Key_F1", Key_F1},
  {"Key_F2", Key_F2},
  {"Key_F3", Key_F3},
  {"Key_F4", Key_F4},
  {"Key_F5", Key_F5},
  {"Key_F6", Key_F6},
  {"Key_F7", Key_F7},
  {"Key_F8", Key_F8},
  {"Key_F9", Key_F9},
  {"Key_F10", Key_F10},
  {"Key_F11", Key_F11},
  {"Key_F12", Key_F12},
  {"Key_F13", Key_F13},
  {"Key_F14", Key_F14},
  {"Key_F15", Key_F15},
  {"Key_F16", Key_F16},
  {"Key_F17", Key_F17},
  {"Key_F18", Key_F18},
  {"Key_F19", Key_F19},
  {"Key_F20", Key_F20},
  {"Key_F21", Key_F21},
  {"Key_F22", Key_F22},
  {"Key_F23", Key_F23},
  {"Key_F24", Key_F24},
  {"Key_F25", Key_F25},
  {"Key_F26", Key_F26},
  {"Key_F27", Key_F27},
  {"Key_F28", Key_F28},
  {"Key_F29", Key_F29},
  {"Key_F30", Key_F30},
  {"Key_F31", Key_F31},
  {"Key_F32", Key_F32},
  {"Key_F33", Key_F33},
  {"Key_F34", Key_F34},
  {"Key_F35", Key_F35},
  {"Key_Super_L", Key_Super_L},
  {"Key_Super_R", Key_Super_R},
  {"Key_Menu", Key_Menu},
  {"Key_Hyper_L", Key_Hyper_L},
  {"Key_Hyper_R", Key_Hyper_R},
  {"Key_Help", Key_Help},
  {"Key_Direction_L", Key_Direction_L},
  {"Key_Direction_R", Key_Direction_R},
  {"Key_Space", Key_Space},
  {"Key_Any", Key_Any},
  {"Key_Exclam", Key_Exclam},
  {"Key_QuoteDbl", Key_QuoteDbl},
  {"Key_NumberSign", Key_NumberSign},
  {"Key_Dollar", Key_Dollar},
  {"Key_Percent", Key_Percent},
  {"Key_Ampersand", Key_Ampersand},
  {"Key_Apostrophe", Key_Apostrophe},
  {"Key_ParenLeft", Key_ParenLeft},
  {"Key_ParenRight", Key_ParenRight},
  {"Key_Asterisk", Key_Asterisk},
  {"Key_Plus", Key_Plus},
  {"Key_Comma", Key_Comma},
  {"Key_Minus", Key_Minus},
  {"Key_Period", Key_Period},
  {"Key_Slash", Key_Slash},
  {"Key_0", Key_0},
  {"Key_1", Key_1},
  {"Key_2", Key_2},
  {"Key_3", Key_3},
  {"Key_4", Key_4},
  {"Key_5", Key_5},
  {"Key_6", Key_6},
  {"Key_7", Key_7},
  {"Key_8", Key_8},
  {"Key_9", Key_9},
  {"Key_Colon", Key_Colon},
  {"Key_Semicolon", Key_Semicolon},
  {"Key_Less", Key_Less},
  {"Key_Equal", Key_Equal},
  {"Key_Greater", Key_Greater},
  {"Key_Question", Key_Question},
  {"Key_At", Key_At},
  {"Key_A", Key_A},
  {"Key_B", Key_B},
  {"Key_C", Key_C},
  {"Key_D", Key_D},
  {"Key_E", Key_E},
  {"Key_F", Key_F},
  {"Key_G", Key_G},
  {"Key_H", Key_H},
  {"Key_I", Key_I},
  {"Key_J", Key_J},
  {"Key_K", Key_K},
  {"Key_L", Key_L},
  {"Key_M", Key_M},
  {"Key_N", Key_N},
  {"Key_O", Key_O},
  {"Key_P", Key_P},
  {"Key_Q", Key_Q},
  {"Key_R", Key_R},
  {"Key_S", Key_S},
  {"Key_T", Key_T},
  {"Key_U", Key_U},
  {"Key_V", Key_V},
  {"Key_W", Key_W},
  {"Key_X", Key_X},
  {"Key_Y", Key_Y},
  {"Key_Z", Key_Z},
  {"Key_BracketLeft", Key_BracketLeft},
  {"Key_Backslash", Key_Backslash},
  {"Key_BracketRight", Key_BracketRight},
  {"Key_AsciiCircum", Key_AsciiCircum},
  {"Key_Underscore", Key_Underscore},
  {"Key_QuoteLeft", Key_QuoteLeft},
  {"Key_BraceLeft", Key_BraceLeft},
  {"Key_Bar", Key_Bar},
  {"Key_BraceRight", Key_BraceRight},
  {"Key_AsciiTilde", Key_AsciiTilde},
  {"Key_nobreakspace", Key_nobreakspace},
  {"Key_exclamdown", Key_exclamdown},
  {"Key_cent", Key_cent},
  {"Key_sterling", Key_sterling},
  {"Key_currency", Key_currency},
  {"Key_yen", Key_yen},
  {"Key_brokenbar", Key_brokenbar},
  {"Key_section", Key_section},
  {"Key_diaeresis", Key_diaeresis},
  {"Key_copyright", Key_copyright},
  {"Key_ordfeminine", Key_ordfeminine},
  {"Key_guillemotleft", Key_guillemotleft},
  {"Key_notsign", Key_notsign},
  {"Key_hyphen", Key_hyphen},
  {"Key_registered", Key_registered},
  {"Key_macron", Key_macron},
  {"Key_degree", Key_degree},
  {"Key_plusminus", Key_plusminus},
  {"Key_twosuperior", Key_twosuperior},
  {"Key_threesuperior", Key_threesuperior},
  {"Key_acute", Key_acute},
  {"Key_mu", Key_mu},
  {"Key_paragraph", Key_paragraph},
  {"Key_periodcentered", Key_periodcentered},
  {"Key_cedilla", Key_cedilla},
  {"Key_onesuperior", Key_onesuperior},
  {"Key_masculine", Key_masculine},
  {"Key_guillemotright", Key_guillemotright},
  {"Key_onequarter", Key_onequarter},
  {"Key_onehalf", Key_onehalf},
  {"Key_threequarters", Key_threequarters},
  {"Key_questiondown", Key_questiondown},
  {"Key_Agrave", Key_Agrave},
  {"Key_Aacute", Key_Aacute},
  {"Key_Acircumflex", Key_Acircumflex},
  {"Key_Atilde", Key_Atilde},
  {"Key_Adiaeresis", Key_Adiaeresis},
  {"Key_Aring", Key_Aring},
  {"Key_AE", Key_AE},
  {"Key_Ccedilla", Key_Ccedilla},
  {"Key_Egrave", Key_Egrave},
  {"Key_Eacute", Key_Eacute},
  {"Key_Ecircumflex", Key_Ecircumflex},
  {"Key_Ediaeresis", Key_Ediaeresis},
  {"Key_Igrave", Key_Igrave},
  {"Key_Iacute", Key_Iacute},
  {"Key_Icircumflex", Key_Icircumflex},
  {"Key_Idiaeresis", Key_Idiaeresis},
  {"Key_ETH", Key_ETH},
  {"Key_Ntilde", Key_Ntilde},
  {"Key_Ograve", Key_Ograve},
  {"Key_Oacute", Key_Oacute},
  {"Key_Ocircumflex", Key_Ocircumflex},
  {"Key_Otilde", Key_Otilde},
  {"Key_Odiaeresis", Key_Odiaeresis},
  {"Key_multiply", Key_multiply},
  {"Key_Ooblique", Key_Ooblique},
  {"Key_Ugrave", Key_Ugrave},
  {"Key_Uacute", Key_Uacute},
  {"Key_Ucircumflex", Key_Ucircumflex},
  {"Key_Udiaeresis", Key_Udiaeresis},
  {"Key_Yacute", Key_Yacute},
  {"Key_THORN", Key_THORN},
  {"Key_ssharp", Key_ssharp},
  {"Key_division", Key_division},
  {"Key_ydiaeresis", Key_ydiaeresis},
  {"Key_Multi_key", Key_Multi_key},
  {"Key_Codeinput", Key_Codeinput},
  {"Key_SingleCandidate", Key_SingleCandidate},
  {"Key_MultipleCandidate", Key_MultipleCandidate},
  {"Key_PreviousCandidate", Key_PreviousCandidate},
  {"Key_Mode_switch", Key_Mode_switch},
  {"Key_Kanji", Key_Kanji},
  {"Key_Muhenkan", Key_Muhenkan},
  {"Key_Henkan", Key_Henkan},
  {"Key_Romaji", Key_Romaji},
  {"Key_Hiragana", Key_Hiragana},
  {"Key_Katakana", Key_Katakana},
  {"Key_Hiragana_Katakana", Key_Hiragana_Katakana},
  {"Key_Zenkaku", Key_Zenkaku},
  {"Key_Hankaku", Key_Hankaku},
  {"Key_Zenkaku_Hankaku", Key_Zenkaku_Hankaku},
  {"Key_Touroku", Key_Touroku},
  {"Key_Massyo", Key_Massyo},
  {"Key_Kana_Lock", Key_Kana_Lock},
  {"Key_Kana_Shift", Key_Kana_Shift},
  {"Key_Eisu_Shift", Key_Eisu_Shift},
  {"Key_Eisu_toggle", Key_Eisu_toggle},
  {"Key_Hangul", Key_Hangul},
  {"Key_Hangul_Start", Key_Hangul_Start},
  {"Key_Hangul_End", Key_Hangul_End},
  {"Key_Hangul_Hanja", Key_Hangul_Hanja},
  {"Key_Hangul_Jamo", Key_Hangul_Jamo},
  {"Key_Hangul_Romaja", Key_Hangul_Romaja},
  {"Key_Hangul_Jeonja", Key_Hangul_Jeonja},
  {"Key_Hangul_Banja", Key_Hangul_Banja},
  {"Key_Hangul_PreHanja", Key_Hangul_PreHanja},
  {"Key_Hangul_PostHanja", Key_Hangul_PostHanja},
  {"Key_Hangul_Special", Key_Hangul_Special},
  {"Key_Dead_Grave", Key_Dead_Grave},
  {"Key_Dead_Acute", Key_Dead_Acute},
  {"Key_Dead_Circumflex", Key_Dead_Circumflex},
  {"Key_Dead_Tilde", Key_Dead_Tilde},
  {"Key_Dead_Macron", Key_Dead_Macron},
  {"Key_Dead_Breve", Key_Dead_Breve},
  {"Key_Dead_Abovedot", Key_Dead_Abovedot},
  {"Key_Dead_Diaeresis", Key_Dead_Diaeresis},
  {"Key_Dead_Abovering", Key_Dead_Abovering},
  {"Key_Dead_Doubleacute", Key_Dead_Doubleacute},
  {"Key_Dead_Caron", Key_Dead_Caron},
  {"Key_Dead_Cedilla", Key_Dead_Cedilla},
  {"Key_Dead_Ogonek", Key_Dead_Ogonek},
  {"Key_Dead_Iota", Key_Dead_Iota},
  {"Key_Dead_Voiced_Sound", Key_Dead_Voiced_Sound},
  {"Key_Dead_Semivoiced_Sound", Key_Dead_Semivoiced_Sound},
  {"Key_Dead_Belowdot", Key_Dead_Belowdot},
  {"Key_Dead_Hook", Key_Dead_Hook},
  {"Key_Dead_Horn", Key_Dead_Horn},
  {"Key_Back", Key_Back},
  {"Key_Forward", Key_Forward},
  {"Key_Stop", Key_Stop},
  {"Key_Refresh", Key_Refresh},
  {"Key_VolumeDown", Key_VolumeDown},
  {"Key_VolumeMute", Key_VolumeMute},
  {"Key_VolumeUp", Key_VolumeUp},
  {"Key_BassBoost", Key_BassBoost},
  {"Key_BassUp", Key_BassUp},
  {"Key_BassDown", Key_BassDown},
  {"Key_TrebleUp", Key_TrebleUp},
  {"Key_TrebleDown", Key_TrebleDown},
  {"Key_MediaPlay", Key_MediaPlay},
  {"Key_MediaStop", Key_MediaStop},
  {"Key_MediaPrevious", Key_MediaPrevious},
  {"Key_MediaNext", Key_MediaNext},
  {"Key_MediaRecord", Key_MediaRecord},
  {"Key_MediaPause", Key_MediaPause},
  {"Key_MediaTogglePlayPause", Key_MediaTogglePlayPause},
  {"Key_HomePage", Key_HomePage},
  {"Key_Favorites", Key_Favorites},
  {"Key_Search", Key_Search},
  {"Key_Standby", Key_Standby},
  {"Key_OpenUrl", Key_OpenUrl},
  {"Key_LaunchMail", Key_LaunchMail},
  {"Key_LaunchMedia", Key_LaunchMedia},
  {"Key_Launch0", Key_Launch0},
  {"Key_Launch1", Key_Launch1},
  {"Key_Launch2", Key_Launch2},
  {"Key_Launch3", Key_Launch3},
  {"Key_Launch4", Key_Launch4},
  {"Key_Launch5", Key_Launch5},
  {"Key_Launch6", Key_Launch6},
  {"Key_Launch7", Key_Launch7},
  {"Key_Launch8", Key_Launch8},
  {"Key_Launch9", Key_Launch9},
  {"Key_LaunchA", Key_LaunchA},
  {"Key_LaunchB", Key_LaunchB},
  {"Key_LaunchC", Key_LaunchC},
  {"Key_LaunchD", Key_LaunchD},
  {"Key_LaunchE", Key_LaunchE},
  {"Key_LaunchF", Key_LaunchF},
  {"Key_LaunchG", Key_LaunchG},
  {"Key_LaunchH", Key_LaunchH},
  {"Key_MonBrightnessUp", Key_MonBrightnessUp},
  {"Key_MonBrightnessDown", Key_MonBrightnessDown},
  {"Key_KeyboardLightOnOff", Key_KeyboardLightOnOff},
  {"Key_KeyboardBrightnessUp", Key_KeyboardBrightnessUp},
  {"Key_KeyboardBrightnessDown", Key_KeyboardBrightnessDown},
  {"Key_PowerOff", Key_PowerOff},
  {"Key_WakeUp", Key_WakeUp},
  {"Key_Eject", Key_Eject},
  {"Key_ScreenSaver", Key_ScreenSaver},
  {"Key_WWW", Key_WWW},
  {"Key_Memo", Key_Memo},
  {"Key_LightBulb", Key_LightBulb},
  {"Key_Shop", Key_Shop},
  {"Key_History", Key_History},
  {"Key_AddFavorite", Key_AddFavorite},
  {"Key_HotLinks", Key_HotLinks},
  {"Key_BrightnessAdjust", Key_BrightnessAdjust},
  {"Key_Finance", Key_Finance},
  {"Key_Community", Key_Community},
  {"Key_AudioRewind", Key_AudioRewind},
  {"Key_BackForward", Key_BackForward},
  {"Key_ApplicationLeft", Key_ApplicationLeft},
  {"Key_ApplicationRight", Key_ApplicationRight},
  {"Key_Book", Key_Book},
  {"Key_CD", Key_CD},
  {"Key_Calculator", Key_Calculator},
  {"Key_ToDoList", Key_ToDoList},
  {"Key_ClearGrab", Key_ClearGrab},
  {"Key_Close", Key_Close},
  {"Key_Copy", Key_Copy},
  {"Key_Cut", Key_Cut},
  {"Key_Display", Key_Display},
  {"Key_DOS", Key_DOS},
  {"Key_Documents", Key_Documents},
  {"Key_Excel", Key_Excel},
  {"Key_Explorer", Key_Explorer},
  {"Key_Game", Key_Game},
  {"Key_Go", Key_Go},
  {"Key_iTouch", Key_iTouch},
  {"Key_LogOff", Key_LogOff},
  {"Key_Market", Key_Market},
  {"Key_Meeting", Key_Meeting},
  {"Key_MenuKB", Key_MenuKB},
  {"Key_MenuPB", Key_MenuPB},
  {"Key_MySites", Key_MySites},
  {"Key_News", Key_News},
  {"Key_OfficeHome", Key_OfficeHome},
  {"Key_Option", Key_Option},
  {"Key_Paste", Key_Paste},
  {"Key_Phone", Key_Phone},
  {"Key_Calendar", Key_Calendar},
  {"Key_Reply", Key_Reply},
  {"Key_Reload", Key_Reload},
  {"Key_RotateWindows", Key_RotateWindows},
  {"Key_RotationPB", Key_RotationPB},
  {"Key_RotationKB", Key_RotationKB},
  {"Key_Save", Key_Save},
  {"Key_Send", Key_Send},
  {"Key_Spell", Key_Spell},
  {"Key_SplitScreen", Key_SplitScreen},
  {"Key_Support", Key_Support},
  {"Key_TaskPane", Key_TaskPane},
  {"Key_Terminal", Key_Terminal},
  {"Key_Tools", Key_Tools},
  {"Key_Travel", Key_Travel},
  {"Key_Video", Key_Video},
  {"Key_Word", Key_Word},
  {"Key_Xfer", Key_Xfer},
  {"Key_ZoomIn", Key_ZoomIn},
  {"Key_ZoomOut", Key_ZoomOut},
  {"Key_Away", Key_Away},
  {"Key_Messenger", Key_Messenger},
  {"Key_WebCam", Key_WebCam},
  {"Key_MailForward", Key_MailForward},
  {"Key_Pictures", Key_Pictures},
  {"Key_Music", Key_Music},
  {"Key_Battery", Key_Battery},
  {"Key_Bluetooth", Key_Bluetooth},
  {"Key_WLAN", Key_WLAN},
  {"Key_UWB", Key_UWB},
  {"Key_AudioForward", Key_AudioForward},
  {"Key_AudioRepeat", Key_AudioRepeat},
  {"Key_AudioRandomPlay", Key_AudioRandomPlay},
  {"Key_Subtitle", Key_Subtitle},
  {"Key_AudioCycleTrack", Key_AudioCycleTrack},
  {"Key_Time", Key_Time},
  {"Key_Hibernate", Key_Hibernate},
  {"Key_View", Key_View},
  {"Key_TopMenu", Key_TopMenu},
  {"Key_PowerDown", Key_PowerDown},
  {"Key_Suspend", Key_Suspend},
  {"Key_ContrastAdjust", Key_ContrastAdjust},
  {"Key_TouchpadToggle", Key_TouchpadToggle},
  {"Key_TouchpadOn", Key_TouchpadOn},
  {"Key_TouchpadOff", Key_TouchpadOff},
  {"Key_MicMute", Key_MicMute},
  {"Key_Red", Key_Red},
  {"Key_Green", Key_Green},
  {"Key_Yellow", Key_Yellow},
  {"Key_Blue", Key_Blue},
  {"Key_ChannelUp", Key_ChannelUp},
  {"Key_ChannelDown", Key_ChannelDown},
  {"Key_Guide", Key_Guide},
  {"Key_Info", Key_Info},
  {"Key_Settings", Key_Settings},
  {"Key_MicVolumeUp", Key_MicVolumeUp},
  {"Key_MicVolumeDown", Key_MicVolumeDown},
  {"Key_New", Key_New},
  {"Key_Open", Key_Open},
  {"Key_Find", Key_Find},
  {"Key_Undo", Key_Undo},
  {"Key_Redo", Key_Redo},
  {"Key_MediaLast", Key_MediaLast},
  {"Key_unknown", Key_unknown},
  {"Key_Call", Key_Call},
  {"Key_Camera", Key_Camera},
  {"Key_CameraFocus", Key_CameraFocus},
  {"Key_Context1", Key_Context1},
  {"Key_Context2", Key_Context2},
  {"Key_Context3", Key_Context3},
  {"Key_Context4", Key_Context4},
  {"Key_Flip", Key_Flip},
  {"Key_Hangup", Key_Hangup},
  {"Key_No", Key_No},
  {"Key_Select", Key_Select},
  {"Key_Yes", Key_Yes},
  {"Key_ToggleCallHangup", Key_ToggleCallHangup},
  {"Key_VoiceDial", Key_VoiceDial},
  {"Key_LastNumberRedial", Key_LastNumberRedial},
  {"Key_Execute", Key_Execute},
  {"Key_Printer", Key_Printer},
  {"Key_Play", Key_Play},
  {"Key_Sleep", Key_Sleep},
  {"Key_Zoom", Key_Zoom},
  {"Key_Exit", Key_Exit},
  {"Key_Cancel", Key_Cancel},
};

#define MAX_KEY_EVENT_NUM 200
#define MAX_KEY_TOP_NUM 100

 /* current section id */
 int section;
 /* next key index */
 int nextkey;
 /* next soft key index */
 int nextsoftkey;

 /* Header */
 KLFHeader header;

 /* table for KeyEvent */
 KeyEvent keyEventTable[MAX_KEY_EVENT_NUM];

 /* table for KeyTop */
 KeyTop keyTopTable[MAX_KEY_TOP_NUM];

 /* error counter */
 int error_count;

%}

%union {
  char *strp;
  int intval;
}

%token <strp>	STRING
%token <strp>	QSTRING
%token <intval>	NUMBER
%token <intval>	SECTION
%token <strp>	KEY_ID
%token <strp>	VK_ID
%token <intval>	SHIFTKEY
%token <intval>	PLATFORM

%%

input:	/* */
		| input line
		;

line:	'\n'
| SECTION {
#ifdef DEBUG_YACC
  printf("Found Section (Section No. = %d)\n", $1);
#endif /* DEBUG_YACC */
  section = $1;
}
| STRING '=' QSTRING {
#ifdef DEBUG_YACC
  printf("ENVVAR = QSTRING (%s = \"%s\")\n", $1, $3);
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_GENERAL){
	// error : NOT in [General]
	error_count++;
	printf("error : NOT in [General]\n");
  }
  else {
	/* set */
	int envvar = getENVVAR_ID((const char*)$1);
	switch(envvar){
	case ID_ENVVAR_NAME:
	  strncpy(header.name, $3, 64-1);
	  break;
	case ID_ENVVAR_AUTHOR:
	  strncpy(header.author, $3, 64-1);
	  break;
	case ID_ENVVAR_SPECVERSION:
	  error_count++;
	  printf("error : ENVVAR(%s) is number. \n", $1);
	  break;
	default:
	  // error
	  error_count++;
	  printf("error : Unknown ENVVAR(%s)\n", $1);
	  break;
	}
  }
}
| STRING '=' NUMBER {
#ifdef DEBUG_YACC
  printf("ENVVAR = NUMBER  (%s = %d)\n", $1, $3);
#endif /* DEBUG_YACC */
  /* section check */
  if (section != ID_SECTION_GENERAL){
	// error : NOT in [Keys]
	error_count++;
	printf("error : NOT in [General]\n");
  }
  else {
	/* set */
	int envvar = getENVVAR_ID((const char*)$1);
	switch(envvar){
	case ID_ENVVAR_NAME:
	case ID_ENVVAR_AUTHOR:
	  // error
	  error_count++;
	  printf("error : ENVVAR(%s) is quoted string. \n", $1);
	  break;
	case ID_ENVVAR_SPECVERSION:
	  header.spec = $3;
	  break;
	default:
	  // error
	  error_count++;
	  printf("error : Unknown ENVVAR(%s)\n", $1);
	  break;
	}
  }
}
| KEY_ID ',' VK_ID ',' SHIFTKEY {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY           : %-25s , %-25s, SHIFTKEY(%d)\n", $1, $3, $5);
  printf("KEY_ID(%s) = 0x%08X\n", $1, getKey_ID((const char*)$1));
  printf("VK_ID(%s) = 0x%02X\n", $3, getVK_ID((const char*)$3));
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_KEYS){
	// error : NOT in [Keys]
	error_count++;
	printf("error : NOT in [Keys]\n");
  }
  else {
	if (nextkey < MAX_KEY_EVENT_NUM){
	  /* set KeyEventTable[nextkey] */

	  Key key = getKey_ID((const char*)$1);
	  int VK_Code = getVK_ID((const char*)$3);
	  ShiftKeyControl shiftKeyControl = $5;

	  /* check */
	  if (key == (Key)-1){
		// Unknown Key
		error_count++;
		printf("error : Unknown Key : %s\n", $1);
	  }
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", $3);
	  }

	  // set KeyEvent
	  keyEventTable[nextkey].key = key;
	  keyEventTable[nextkey].VK_Code = VK_Code;
	  keyEventTable[nextkey].shiftKeyControl = shiftKeyControl;

	  nextkey++;
	}
	else {
	  // error : too many key event entry
	  error_count++;
	  printf("error : too many key event entry in [Keys]\n");
	}
  }
}
| KEY_ID ',' VK_ID ',' SHIFTKEY ',' PLATFORM {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY, PLATFORM : %-25s , %-25s, SHIFTKEY(%d), PLATFORM(%d)\n", $1, $3, $5, $7);
  printf("KEY_ID(%s) = 0x%08X\n", $1, getKey_ID((const char*)$1));
  printf("VK_ID(%s) = 0x%02X\n", $3, getVK_ID((const char*)$3));
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_KEYS){
	// error : NOT in [Keys]
	error_count++;
	printf("error : NOT in [Keys]\n");
  }
  else {
	/* override if this_platform == PLATFORM */
	if (this_platform == $7){
	  Key key = getKey_ID((const char*)$1);
	  int VK_Code = getVK_ID((const char*)$3);
	  ShiftKeyControl shiftKeyControl = $5;
	  int index;

	  /* 1) search index of key event entry */
	  index = getID_Key(key);

	  /* 2) overwrite */
	  if (index >= 0){
		keyEventTable[index].key = key;
		keyEventTable[index].VK_Code = VK_Code;
		keyEventTable[index].shiftKeyControl = shiftKeyControl;
	  }
	  else {
		// Not Found Key for overwrite
		error_count++;
		printf("error : Not Found Key : %s\n", $1);
	  }
	}
  }
}
| NUMBER ',' QSTRING ',' QSTRING ',' VK_ID ',' QSTRING ',' VK_ID {
#ifdef DEBUG_YACC
  printf("NUMBER, QSTRING, QSTRING, VK_ID, QSTRING, VK_ID : ");
  printf("%2d, %-10s, %-10s, %-25s, %-10s, %s\n", $1, $3, $5, $7, $9, $11);
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_SOFTKEYS){
	// error : NOT in [SoftKeys]
	error_count++;
	printf("error : NOT in [SoftKeys]\n");
  }
  else {
	if (nextsoftkey < MAX_KEY_TOP_NUM){
	  int VK_Code;

	  /* set keTopTable[$1-1] */
	  KeyTop *keyTop = &keyTopTable[$1-1];

	  /* check */
	  if (strlen($3) > MAX_KEYTOP_STRING){
		// too long string
		printf("error : too long string : %s\n", $3);
		error_count++;
	  }
	  if (strlen($5) > MAX_KEYTOP_STRING){
		// too long string
		error_count++;
		printf("error : too long string : %s\n", $5);
	  }
	  VK_Code = getVK_ID($7);
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", $7);
	  }
	  if (strlen($9) > MAX_KEYTOP_STRING){
		// too long string
		error_count++;
		printf("error : too long string : %s\n", $9);
	  }
	  VK_Code = getVK_ID($11);
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", $11);
	  }

	  // set KeyTop
	  strncpy(keyTop->keyTop.keyTop, $3, MAX_KEYTOP_STRING);
	  strncpy(keyTop->keyTop.keyTopWithShift, $5, MAX_KEYTOP_STRING);
	  keyTop->keyTop.VK_Code = getVK_ID($7);
	  strncpy(keyTop->keyTopWithFn.keyTop, $9, MAX_KEYTOP_STRING);
	  keyTop->keyTopWithFn.VK_Code = getVK_ID($11);

	  nextsoftkey++;
	}
	else {
	  // error : too many key top entry
	  error_count++;
	  printf("error : too many key top entry in [SoftKeys]\n");
	}
  }
}
| error '\n' {
  // error
  error_count++;
}
;

%%

/* get envvar by name */
int getENVVAR_ID(const char *name)
{
  int i;
  for(i = 0; i < ID_ENVVAR_NUM; i++){
	if (strcmp(envvarname[i], name) == 0){
	  return i;
	}
  }

  return -1;
}

/* get vk_code by name */
int getVK_ID(const char *name)
{
  int i;
  for(i = 0; i < 256; i++){
	if (strcmp(vkname[i], name) == 0){
	  return i;
	}
  }

  return -1;
}

/* get Key by name */
Key getKey_ID(const char *name)
{
  int i;
  for(i = 0; i < KEY_ENTRY_NUM; i++){
	if (strcmp(keys[i].name, name) == 0){
	  return keys[i].key;
	}
  }

  return -1;
}

int getID_Key(Key key)
{
  int i;
  for(i = 0; i < nextkey; i++){
	if (keyEventTable[i].key == key){
	  return i;
	}
  }

  return -1;
}

// .kl to .klx
int make_KLX(const char *infile, const char *outfile)
{
  extern FILE *yyin;
  FILE *fp;
  size_t total = 0;
  size_t result = 0;

  section = -1;
  nextkey = 0;
  nextsoftkey = 0;
  error_count = 0;

  // zero clear of header, tables
  memset(&header, 0, sizeof(header));
  memset(keyEventTable, 0, sizeof(keyEventTable));
  memset(keyTopTable, 0, sizeof(keyTopTable));

  /* open .kl file */
  if (infile != NULL){
	yyin = fopen(infile, "r");
	if (yyin == NULL){
	  // open failed
	  return -1;
	}
  }

  /* read .kl file */
  yyparse();

#ifdef DEBUG_YACC
  printf("sizeof(KLFHeader) = %d\n", (int)sizeof(KLFHeader));
  printf("sizeof(KeyEvent)  = %d\n", (int)sizeof(KeyEvent));
  printf("sizeof(KeyTop)    = %d\n", (int)sizeof(KeyTop));
#endif /* DEBUG_YACC */

  total = sizeof(KLFHeader) + sizeof(KeyEvent) * nextkey + sizeof(KeyTop) * nextsoftkey;

#ifdef DEBUG_YACC
  printf("==== RESULT ====\n");
  printf("keynum     = %d\n", nextkey);
  printf("softkeynum = %d\n", nextsoftkey);
  printf("total size = %d\n", (int)total);
#endif /* DEBUG_YACC */

  /* close .kl file */
  //  fclose(yyin);

  if (outfile == NULL){
	return 0;
  }

  /* make file header */
  strncpy(header.magic, "KLF", 3);
  header.size = (int)total;
  header.keynum = nextkey;
  header.softkeynum = nextsoftkey;

  /* open .klx file */
  fp = fopen(outfile, "wb");
  if (fp == NULL){
	// error
	return -1;
  }

  /* write .klx file */
  result += fwrite((const char *)&header, sizeof(KLFHeader), 1, fp) * sizeof(KLFHeader);
  result += fwrite((const char *)keyEventTable, sizeof(KeyEvent), nextkey, fp) * sizeof(KeyEvent);
  result += fwrite((const char *)keyTopTable, sizeof(KeyTop), nextsoftkey, fp) * sizeof(KeyTop);

  /* close .klx file */
  fclose(fp);

  if (result != total){
	/* error : write error */
	printf("error : Write error.\n");
	return -1;
  }
  else {
	/* O.K. */
	/* printf("output : %s\n", outfile); */
  }

  return error_count;
}

void yyerror(char *msg)
{
  printf("%d : %s\n", lineno, msg);
}
