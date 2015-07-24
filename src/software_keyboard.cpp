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

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboard::SoftwareKeyboard(QWidget *parent)
										   
  :
  // for DEBUG
  outputLog(true)
{
  setupUi(this);
}

} // end of namespace qtbrynhildr
