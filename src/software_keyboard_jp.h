// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_keybord_jp.h

#ifndef SOFTWARE_KEYBOARD_JP_H
#define SOFTWARE_KEYBOARD_JP_H
// Qt Header

// Local Header
#include "software_keyboard.h"

namespace qtbrynhildr {

  // key top text table
  const static string keyTopText[SoftwareKeyboard::ID_KEY_NUM] = {
	"ESC","1","2","3","4","5","6","7","8","9","0","-","^","\\","BS",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","@","[","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L",";",":","]",
	"Shift","Z","X","C","V","B","N","M",",",".","/","\\","\342\206\221","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","\342\206\220","\342\206\223","\342\206\222"
  };

  const static string keyTopTextWithShift[SoftwareKeyboard::ID_KEY_NUM] = {
	"ESC","!","\"","#","$","%","&","'","(",")","0","=","~","|","BS",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","`","{","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L","+","*","}",
	"Shift","Z","X","C","V","B","N","M","<",">","?","_","\342\206\221","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","\342\206\220","\342\206\223","\342\206\222"
  };

  const static string keyTopTextWithFn[SoftwareKeyboard::ID_KEY_NUM] = {
	"ESC","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","-","^","\\","BS",
	"Tab","Q","W","E","R","T","Y","U","I","O","P","@","[","RET",
	"Ctrl","A","S","D","F","G","H","J","K","L",";",":","]",
	"Shift","Z","X","C","V","B","N","M",",",".","/","\\","\342\206\221","Shift",
	"Fn","Alt","Windows","JP","Space","Ctrl","Alt","Fn","\342\206\220","\342\206\223","\342\206\222"
  };

// SoftwareKeyboard_JP
class SoftwareKeyboard_JP : public SoftwareKeyboard
{
public:
  // constructor
  SoftwareKeyboard_JP(KeyBuffer *keyBuffer, QWidget *parent = 0);

protected:
  // pressed key
  void pressedKey(ID_KEY id);

  // released key
  void releasedKey(ID_KEY id);

private:
  // initialize key top text
  void initializeKeyTopText();

  // shift key
  void pressedShiftKey();

  // Fn key
  void pressedFnKey();
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_JP_H
