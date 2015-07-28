// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_keyboard.h

#ifndef SOFTWARE_KEYBOARD_H
#define SOFTWARE_KEYBOARD_H
// System Header
#include <string>

// Qt Header
#include <QWidget>

// Local Header
#include "keybuffer.h"
#include "settings.h"
#include "ui_software_keyboard.h"

namespace qtbrynhildr {

// SoftwareKeyboard
class SoftwareKeyboard : public QWidget, public Ui::SoftwareKeyboard
{
  Q_OBJECT

public:
  // constructor
  SoftwareKeyboard(Settings *settings, KeyBuffer *keyBuffer, QWidget *parent = 0);

  // key id
  typedef enum {
	ID_KEY_1 = 1,
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
	ID_KEY_NUM = ID_KEY_67
  } ID_KEY;

protected:
  // pressed key
  virtual void pressedKey(ID_KEY id) = 0;

  // released key
  virtual void releasedKey(ID_KEY id) = 0;

  // get name of virtual keycode
  string getVKCodeByString(uchar vkcode);

private slots:
  // pressed slots
  void pressedKey_1();
  void pressedKey_2();
  void pressedKey_3();
  void pressedKey_4();
  void pressedKey_5();
  void pressedKey_6();
  void pressedKey_7();
  void pressedKey_8();
  void pressedKey_9();
  void pressedKey_10();
  void pressedKey_11();
  void pressedKey_12();
  void pressedKey_13();
  void pressedKey_14();
  void pressedKey_15();
  void pressedKey_16();
  void pressedKey_17();
  void pressedKey_18();
  void pressedKey_19();
  void pressedKey_20();
  void pressedKey_21();
  void pressedKey_22();
  void pressedKey_23();
  void pressedKey_24();
  void pressedKey_25();
  void pressedKey_26();
  void pressedKey_27();
  void pressedKey_28();
  void pressedKey_29();
  void pressedKey_30();
  void pressedKey_31();
  void pressedKey_32();
  void pressedKey_33();
  void pressedKey_34();
  void pressedKey_35();
  void pressedKey_36();
  void pressedKey_37();
  void pressedKey_38();
  void pressedKey_39();
  void pressedKey_40();
  void pressedKey_41();
  void pressedKey_42();
  void pressedKey_43();
  void pressedKey_44();
  void pressedKey_45();
  void pressedKey_46();
  void pressedKey_47();
  void pressedKey_48();
  void pressedKey_49();
  void pressedKey_50();
  void pressedKey_51();
  void pressedKey_52();
  void pressedKey_53();
  void pressedKey_54();
  void pressedKey_55();
  void pressedKey_56();
  void pressedKey_57();
  void pressedKey_58();
  void pressedKey_59();
  void pressedKey_60();
  void pressedKey_61();
  void pressedKey_62();
  void pressedKey_63();
  void pressedKey_64();
  void pressedKey_65();
  void pressedKey_66();
  void pressedKey_67();

  // released slots
  void releasedKey_1();
  void releasedKey_2();
  void releasedKey_3();
  void releasedKey_4();
  void releasedKey_5();
  void releasedKey_6();
  void releasedKey_7();
  void releasedKey_8();
  void releasedKey_9();
  void releasedKey_10();
  void releasedKey_11();
  void releasedKey_12();
  void releasedKey_13();
  void releasedKey_14();
  void releasedKey_15();
  void releasedKey_16();
  void releasedKey_17();
  void releasedKey_18();
  void releasedKey_19();
  void releasedKey_20();
  void releasedKey_21();
  void releasedKey_22();
  void releasedKey_23();
  void releasedKey_24();
  void releasedKey_25();
  void releasedKey_26();
  void releasedKey_27();
  void releasedKey_28();
  void releasedKey_29();
  void releasedKey_30();
  void releasedKey_31();
  void releasedKey_32();
  void releasedKey_33();
  void releasedKey_34();
  void releasedKey_35();
  void releasedKey_36();
  void releasedKey_37();
  void releasedKey_38();
  void releasedKey_39();
  void releasedKey_40();
  void releasedKey_41();
  void releasedKey_42();
  void releasedKey_43();
  void releasedKey_44();
  void releasedKey_45();
  void releasedKey_46();
  void releasedKey_47();
  void releasedKey_48();
  void releasedKey_49();
  void releasedKey_50();
  void releasedKey_51();
  void releasedKey_52();
  void releasedKey_53();
  void releasedKey_54();
  void releasedKey_55();
  void releasedKey_56();
  void releasedKey_57();
  void releasedKey_58();
  void releasedKey_59();
  void releasedKey_60();
  void releasedKey_61();
  void releasedKey_62();
  void releasedKey_63();
  void releasedKey_64();
  void releasedKey_65();
  void releasedKey_66();
  void releasedKey_67();

protected:
  // settings
  Settings *settings;

  // key buffer
  KeyBuffer *keyBuffer;

  // shift key status
  bool onShiftKey;

  // control key status
  bool onControlKey;

  // alt key status
  bool onAltKey;

  // Fn key status
  bool onFnKey;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_H
