// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard_dialog.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>
#include <QHBoxLayout>

// Local Header
#include "software_keyboard_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareKeyboardDialog::SoftwareKeyboardDialog(SoftwareKeyboard *softwareKeyboard,
											   QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  softwareKeyboard(softwareKeyboard)
{
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(softwareKeyboard);
  setLayout(layout);
  resize(softwareKeyboard->size());
  setWindowTitle("Software Keyboard - Test -");
}

} // end of namespace qtbrynhildr
