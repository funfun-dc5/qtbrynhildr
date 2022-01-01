/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 4 "klfcompiler.y"

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


#line 871 "klfcompiler.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "klfcompiler.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STRING = 3,                     /* STRING  */
  YYSYMBOL_QSTRING = 4,                    /* QSTRING  */
  YYSYMBOL_NUMBER = 5,                     /* NUMBER  */
  YYSYMBOL_SECTION = 6,                    /* SECTION  */
  YYSYMBOL_KEY_ID = 7,                     /* KEY_ID  */
  YYSYMBOL_VK_ID = 8,                      /* VK_ID  */
  YYSYMBOL_SHIFTKEY = 9,                   /* SHIFTKEY  */
  YYSYMBOL_PLATFORM = 10,                  /* PLATFORM  */
  YYSYMBOL_11_n_ = 11,                     /* '\n'  */
  YYSYMBOL_12_ = 12,                       /* '='  */
  YYSYMBOL_13_ = 13,                       /* ','  */
  YYSYMBOL_YYACCEPT = 14,                  /* $accept  */
  YYSYMBOL_input = 15,                     /* input  */
  YYSYMBOL_line = 16                       /* line  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   28

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  14
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  3
/* YYNRULES -- Number of rules.  */
#define YYNRULES  11
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  30

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   265


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      11,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    13,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    12,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   820,   820,   821,   824,   825,   831,   864,   895,   942,
     980,  1044
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "QSTRING",
  "NUMBER", "SECTION", "KEY_ID", "VK_ID", "SHIFTKEY", "PLATFORM", "'\\n'",
  "'='", "','", "$accept", "input", "line", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-10)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -10,     0,   -10,    -9,    -8,    -3,   -10,    -1,   -10,   -10,
     -10,     4,     9,     6,   -10,   -10,     2,     3,    13,    10,
       5,     7,    14,    11,    12,   -10,    19,    15,    16,   -10
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     5,     0,     4,     3,
      11,     0,     0,     0,     6,     7,     0,     0,     0,     0,
       0,     8,     0,     0,     0,     9,     0,     0,     0,    10
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -10,   -10,   -10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     9
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       2,     3,    10,     4,    11,     5,     6,     7,    14,    15,
      12,     8,    13,    16,    17,    18,    19,    20,    22,    21,
      23,    25,    24,    27,    29,    26,     0,     0,    28
};

static const yytype_int8 yycheck[] =
{
       0,     1,    11,     3,    12,     5,     6,     7,     4,     5,
      13,    11,    13,     4,     8,    13,    13,     4,    13,     9,
      13,    10,     8,     4,     8,    13,    -1,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    15,     0,     1,     3,     5,     6,     7,    11,    16,
      11,    12,    13,    13,     4,     5,     4,     8,    13,    13,
       4,     9,    13,    13,     8,    10,    13,     4,    13,     8
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    14,    15,    15,    16,    16,    16,    16,    16,    16,
      16,    16
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     3,     3,     5,     7,
      11,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5: /* line: SECTION  */
#line 825 "klfcompiler.y"
          {
#ifdef DEBUG_YACC
  printf("Found Section (Section No. = %d)\n", (yyvsp[0].intval));
#endif /* DEBUG_YACC */
  section = (yyvsp[0].intval);
}
#line 1880 "klfcompiler.tab.c"
    break;

  case 6: /* line: STRING '=' QSTRING  */
#line 831 "klfcompiler.y"
                     {
#ifdef DEBUG_YACC
  printf("ENVVAR = QSTRING (%s = \"%s\")\n", (yyvsp[-2].strp), (yyvsp[0].strp));
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_GENERAL){
	// error : NOT in [General]
	error_count++;
	printf("error : NOT in [General]\n");
  }
  else {
	/* set */
	int envvar = getENVVAR_ID((const char*)(yyvsp[-2].strp));
	switch(envvar){
	case ID_ENVVAR_NAME:
	  strncpy(header.name, (yyvsp[0].strp), 64-1);
	  break;
	case ID_ENVVAR_AUTHOR:
	  strncpy(header.author, (yyvsp[0].strp), 64-1);
	  break;
	case ID_ENVVAR_SPECVERSION:
	  error_count++;
	  printf("error : ENVVAR(%s) is number. \n", (yyvsp[-2].strp));
	  break;
	default:
	  // error
	  error_count++;
	  printf("error : Unknown ENVVAR(%s)\n", (yyvsp[-2].strp));
	  break;
	}
  }
}
#line 1918 "klfcompiler.tab.c"
    break;

  case 7: /* line: STRING '=' NUMBER  */
#line 864 "klfcompiler.y"
                    {
#ifdef DEBUG_YACC
  printf("ENVVAR = NUMBER  (%s = %d)\n", (yyvsp[-2].strp), (yyvsp[0].intval));
#endif /* DEBUG_YACC */
  /* section check */
  if (section != ID_SECTION_GENERAL){
	// error : NOT in [Keys]
	error_count++;
	printf("error : NOT in [General]\n");
  }
  else {
	/* set */
	int envvar = getENVVAR_ID((const char*)(yyvsp[-2].strp));
	switch(envvar){
	case ID_ENVVAR_NAME:
	case ID_ENVVAR_AUTHOR:
	  // error
	  error_count++;
	  printf("error : ENVVAR(%s) is quoted string. \n", (yyvsp[-2].strp));
	  break;
	case ID_ENVVAR_SPECVERSION:
	  header.spec = (yyvsp[0].intval);
	  break;
	default:
	  // error
	  error_count++;
	  printf("error : Unknown ENVVAR(%s)\n", (yyvsp[-2].strp));
	  break;
	}
  }
}
#line 1954 "klfcompiler.tab.c"
    break;

  case 8: /* line: KEY_ID ',' VK_ID ',' SHIFTKEY  */
#line 895 "klfcompiler.y"
                                {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY           : %-25s , %-25s, SHIFTKEY(%d)\n", (yyvsp[-4].strp), (yyvsp[-2].strp), (yyvsp[0].intval));
  printf("KEY_ID(%s) = 0x%08X\n", (yyvsp[-4].strp), getKey_ID((const char*)(yyvsp[-4].strp)));
  printf("VK_ID(%s) = 0x%02X\n", (yyvsp[-2].strp), getVK_ID((const char*)(yyvsp[-2].strp)));
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

	  Key key = getKey_ID((const char*)(yyvsp[-4].strp));
	  int VK_Code = getVK_ID((const char*)(yyvsp[-2].strp));
	  ShiftKeyControl shiftKeyControl = (yyvsp[0].intval);

	  /* check */
	  if (key == (Key)-1){
		// Unknown Key
		error_count++;
		printf("error : Unknown Key : %s\n", (yyvsp[-4].strp));
	  }
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", (yyvsp[-2].strp));
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
#line 2006 "klfcompiler.tab.c"
    break;

  case 9: /* line: KEY_ID ',' VK_ID ',' SHIFTKEY ',' PLATFORM  */
#line 942 "klfcompiler.y"
                                             {
#ifdef DEBUG_YACC
  printf("KEY_ID , VK_ID, SHIFTKEY, PLATFORM : %-25s , %-25s, SHIFTKEY(%d), PLATFORM(%d)\n", (yyvsp[-6].strp), (yyvsp[-4].strp), (yyvsp[-2].intval), (yyvsp[0].intval));
  printf("KEY_ID(%s) = 0x%08X\n", (yyvsp[-6].strp), getKey_ID((const char*)(yyvsp[-6].strp)));
  printf("VK_ID(%s) = 0x%02X\n", (yyvsp[-4].strp), getVK_ID((const char*)(yyvsp[-4].strp)));
#endif /* DEBUG_YACC */

  /* section check */
  if (section != ID_SECTION_KEYS){
	// error : NOT in [Keys]
	error_count++;
	printf("error : NOT in [Keys]\n");
  }
  else {
	/* override if this_platform == PLATFORM */
	if (this_platform == (yyvsp[0].intval)){
	  Key key = getKey_ID((const char*)(yyvsp[-6].strp));
	  int VK_Code = getVK_ID((const char*)(yyvsp[-4].strp));
	  ShiftKeyControl shiftKeyControl = (yyvsp[-2].intval);
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
		printf("error : Not Found Key : %s\n", (yyvsp[-6].strp));
	  }
	}
  }
}
#line 2049 "klfcompiler.tab.c"
    break;

  case 10: /* line: NUMBER ',' QSTRING ',' QSTRING ',' VK_ID ',' QSTRING ',' VK_ID  */
#line 980 "klfcompiler.y"
                                                                 {
#ifdef DEBUG_YACC
  printf("NUMBER, QSTRING, QSTRING, VK_ID, QSTRING, VK_ID : ");
  printf("%2d, %-10s, %-10s, %-25s, %-10s, %s\n", (yyvsp[-10].intval), (yyvsp[-8].strp), (yyvsp[-6].strp), (yyvsp[-4].strp), (yyvsp[-2].strp), (yyvsp[0].strp));
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
	  KeyTop *keyTop = &keyTopTable[(yyvsp[-10].intval)-1];

	  /* check */
	  if (strlen((yyvsp[-8].strp)) > MAX_KEYTOP_STRING){
		// too long string
		printf("error : too long string : %s\n", (yyvsp[-8].strp));
		error_count++;
	  }
	  if (strlen((yyvsp[-6].strp)) > MAX_KEYTOP_STRING){
		// too long string
		error_count++;
		printf("error : too long string : %s\n", (yyvsp[-6].strp));
	  }
	  VK_Code = getVK_ID((yyvsp[-4].strp));
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", (yyvsp[-4].strp));
	  }
	  if (strlen((yyvsp[-2].strp)) > MAX_KEYTOP_STRING){
		// too long string
		error_count++;
		printf("error : too long string : %s\n", (yyvsp[-2].strp));
	  }
	  VK_Code = getVK_ID((yyvsp[0].strp));
	  if (VK_Code == -1){
		// Unknown VK Code
		error_count++;
		printf("error : Unknown VK Code : %s\n", (yyvsp[0].strp));
	  }

	  // set KeyTop
	  strncpy(keyTop->keyTop.keyTop, (yyvsp[-8].strp), MAX_KEYTOP_STRING);
	  strncpy(keyTop->keyTop.keyTopWithShift, (yyvsp[-6].strp), MAX_KEYTOP_STRING);
	  keyTop->keyTop.VK_Code = getVK_ID((yyvsp[-4].strp));
	  strncpy(keyTop->keyTopWithFn.keyTop, (yyvsp[-2].strp), MAX_KEYTOP_STRING);
	  keyTop->keyTopWithFn.VK_Code = getVK_ID((yyvsp[0].strp));

	  nextsoftkey++;
	}
	else {
	  // error : too many key top entry
	  error_count++;
	  printf("error : too many key top entry in [SoftKeys]\n");
	}
  }
}
#line 2118 "klfcompiler.tab.c"
    break;

  case 11: /* line: error '\n'  */
#line 1044 "klfcompiler.y"
             {
  // error
  error_count++;
}
#line 2127 "klfcompiler.tab.c"
    break;


#line 2131 "klfcompiler.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1050 "klfcompiler.y"


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
