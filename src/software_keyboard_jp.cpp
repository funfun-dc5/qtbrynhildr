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
  // update key top text
  updateKeyTopText();
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------
// pressed key
void SoftwareKeyboard_JP::pressedKey(ID_KEY id)
{
  if (outputLog)
	cout << "Pressed Key! id = " << id << endl << flush;

  switch(id){
  case ID_KEY_1:
	// VK_ESCAPE
	cout << "Pressed : VK_ESCAPE" << endl << flush;
	break;
  default:
	// unknown
	break;
  }
}

// released key
void SoftwareKeyboard_JP::releasedKey(ID_KEY id)
{
  if (outputLog)
	cout << "Released Key! id = " << id << endl << flush;
}

// update key top text
void SoftwareKeyboard_JP::updateKeyTopText()
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

} // end of namespace qtbrynhildr
