// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard_jp.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_keyboard_jp.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
SoftwareKeyboard_JP::SoftwareKeyboard_JP(KeyBuffer *keyBuffer, QWidget *parent)
  :
  SoftwareKeyboard(keyBuffer, parent)
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
	  cout << "Pressed : " << keyTopTextWithShift[id] << endl << flush;
	else if (onFnKey)
	  cout << "Pressed : " << keyTopTextWithFn[id] << endl << flush;
	else
	  cout << "Pressed : " << keyTopText[id] << endl << flush;
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
  case ID_KEY_59:
	// Windows
	onWindowsKey = !onWindowsKey;
	cout << "onWindowsKey : " << onWindowsKey << endl << flush;
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
	  switch(id){
	  case ID_KEY_2:	// F1
	  case ID_KEY_3:	// F2
	  case ID_KEY_4:	// F3
	  case ID_KEY_5:	// F4
	  case ID_KEY_6:	// F5
	  case ID_KEY_7:	// F6
	  case ID_KEY_8:	// F7
	  case ID_KEY_9:	// F8
	  case ID_KEY_10:	// F9
	  case ID_KEY_11:	// F10
		// send Fn key
		keyBuffer->put(VK_F1 + (id - ID_KEY_2), KEYCODE_FLG_KEYDOWN);
		break;
	  default:
		// Nothing to do
		break;
	  }
	  onFnKey = false; // Just for once
	}
	else { // except for Fn keys
	  if (onShiftKey){
		keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYDOWN);
	  }
	  if (onControlKey){
		keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYDOWN);
		onControlKey = false; // Just for once
	  }
	  if (onAltKey){
		keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYDOWN);
		onAltKey = false; // Just for once
	  }
	  if (onWindowsKey){
		keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYDOWN);
		onWindowsKey = false; // Just for once
	  }
	  keyBuffer->put(VK_Code[id], KEYCODE_FLG_KEYDOWN);
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
	  cout << "Released : " << keyTopTextWithShift[id] << endl << flush;
	else if (onFnKey)
	  cout << "Released : " << keyTopTextWithFn[id] << endl << flush;
	else
	  cout << "Released : " << keyTopText[id] << endl << flush;
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
  case ID_KEY_30:
  case ID_KEY_62:
	// Control
  case ID_KEY_58:
  case ID_KEY_63:
	// Alt
  case ID_KEY_59:
	// Windows
  case ID_KEY_57:
  case ID_KEY_64:
	// Fn
	// Nothig to do
	break;
  default:
	if (onFnKey){
	  // Fn keys
	}
	else { // except for Fn keys
	  keyBuffer->put(VK_Code[id], KEYCODE_FLG_KEYUP);
	  if (onWindowsKey){
		keyBuffer->put(VK_LWIN, KEYCODE_FLG_KEYUP);
	  }
	  if (onAltKey){
		keyBuffer->put(VK_MENU, KEYCODE_FLG_KEYUP);
	  }
	  if (onControlKey){
		keyBuffer->put(VK_CONTROL, KEYCODE_FLG_KEYUP);
	  }
	  if (onShiftKey){
		keyBuffer->put(VK_SHIFT, KEYCODE_FLG_KEYUP);
	  }
	}
	break;
  }
}

// update key top text
void SoftwareKeyboard_JP::initializeKeyTopText()
{
  pushButton_1->setText("ESC");
  pushButton_2->setText("1");
  pushButton_3->setText("2");
  pushButton_4->setText("3");
  pushButton_5->setText("4");
  pushButton_6->setText("5");
  pushButton_7->setText("6");
  pushButton_8->setText("7");
  pushButton_9->setText("8");
  pushButton_10->setText("9");
  pushButton_11->setText("0");
  pushButton_12->setText("-");
  pushButton_13->setText("^");
  pushButton_14->setText("\\");
  pushButton_15->setText("BS");
  pushButton_16->setText("Tab");
  pushButton_17->setText("Q");
  pushButton_18->setText("W");
  pushButton_19->setText("E");
  pushButton_20->setText("R");
  pushButton_21->setText("T");
  pushButton_22->setText("Y");
  pushButton_23->setText("U");
  pushButton_24->setText("I");
  pushButton_25->setText("O");
  pushButton_26->setText("P");
  pushButton_27->setText("@");
  pushButton_28->setText("[");
  pushButton_29->setText("RET");
  pushButton_30->setText("Ctrl");
  pushButton_31->setText("A");
  pushButton_32->setText("S");
  pushButton_33->setText("D");
  pushButton_34->setText("F");
  pushButton_35->setText("G");
  pushButton_36->setText("H");
  pushButton_37->setText("J");
  pushButton_38->setText("K");
  pushButton_39->setText("L");
  pushButton_40->setText(";");
  pushButton_41->setText(":");
  pushButton_42->setText("]");
  pushButton_43->setText("Shift");
  pushButton_44->setText("Z");
  pushButton_45->setText("X");
  pushButton_46->setText("C");
  pushButton_47->setText("V");
  pushButton_48->setText("B");
  pushButton_49->setText("N");
  pushButton_50->setText("M");
  pushButton_51->setText(",");
  pushButton_52->setText(".");
  pushButton_53->setText("/");
  pushButton_54->setText("\\");
  pushButton_55->setText("\342\206\221");
  pushButton_56->setText("Shift");
  pushButton_57->setText("Fn");
  pushButton_58->setText("Alt");
  pushButton_59->setText("Windows");
  pushButton_60->setText("JP");
  pushButton_61->setText("Space");
  pushButton_62->setText("Ctrl");
  pushButton_63->setText("Alt");
  pushButton_64->setText("Fn");
  pushButton_65->setText("\342\206\220");
  pushButton_66->setText("\342\206\223");
  pushButton_67->setText("\342\206\222");
}

// shift key
void SoftwareKeyboard_JP::pressedShiftKey()
{
  if (onShiftKey){
	// pressed
	pushButton_2->setText("!");
	pushButton_3->setText("\"");
	pushButton_4->setText("#");
	pushButton_5->setText("$");
	pushButton_6->setText("%");
	pushButton_7->setText("&&");
	pushButton_8->setText("'");
	pushButton_9->setText("(");
	pushButton_10->setText(")");
	pushButton_11->setText("");
	pushButton_12->setText("=");
	pushButton_13->setText("~");
	pushButton_14->setText("|");
	pushButton_27->setText("`");
	pushButton_28->setText("{");
	pushButton_40->setText("+");
	pushButton_41->setText("*");
	pushButton_42->setText("}");
	pushButton_51->setText("<");
	pushButton_52->setText(">");
	pushButton_53->setText("?");
	pushButton_54->setText("_");
  }
  else {
	// released
	pushButton_2->setText("1");
	pushButton_3->setText("2");
	pushButton_4->setText("3");
	pushButton_5->setText("4");
	pushButton_6->setText("5");
	pushButton_7->setText("6");
	pushButton_8->setText("7");
	pushButton_9->setText("8");
	pushButton_10->setText("9");
	pushButton_11->setText("0");
	pushButton_12->setText("-");
	pushButton_13->setText("^");
	pushButton_14->setText("\\");
	pushButton_27->setText("@");
	pushButton_28->setText("[");
	pushButton_40->setText(";");
	pushButton_41->setText(":");
	pushButton_42->setText("]");
	pushButton_51->setText(",");
	pushButton_52->setText(".");
	pushButton_53->setText("/");
	pushButton_54->setText("\\");
  }
}

// Fn key
void SoftwareKeyboard_JP::pressedFnKey()
{
  if (onFnKey){
	// pressed
	pushButton_2->setText("F1");
	pushButton_3->setText("F2");
	pushButton_4->setText("F3");
	pushButton_5->setText("F4");
	pushButton_6->setText("F5");
	pushButton_7->setText("F6");
	pushButton_8->setText("F7");
	pushButton_9->setText("F8");
	pushButton_10->setText("F9");
	pushButton_11->setText("F10");
  }
  else {
	// released
	pushButton_2->setText("1");
	pushButton_3->setText("2");
	pushButton_4->setText("3");
	pushButton_5->setText("4");
	pushButton_6->setText("5");
	pushButton_7->setText("6");
	pushButton_8->setText("7");
	pushButton_9->setText("8");
	pushButton_10->setText("9");
	pushButton_11->setText("0");
  }
}

} // end of namespace qtbrynhildr
