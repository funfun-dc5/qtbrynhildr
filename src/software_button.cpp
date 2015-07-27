// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_button.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header

// Local Header
#include "software_button.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareButton::SoftwareButton(MouseBuffer *mouseBuffer, QWidget *parent)
  :
  mouseBuffer(mouseBuffer),
  // for DEBUG
  outputLog(true)
{
  setupUi(this);

  // connect signals
  // pressed signals
  //  connect(pushButton_1, SIGNAL(pressed()), this, SLOT(pressedKey_1()));

  // released signals
  //  connect(pushButton_1, SIGNAL(released()), this, SLOT(releasedKey_1()));
}

//---------------------------------------------------------------------------
// protected
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------

} // end of namespace qtbrynhildr
