// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_keyboard.h"
#include "windows/keycodes.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboard::SoftwareKeyboard(KeyBuffer *keyBuffer, QWidget *parent)
  :
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
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

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
