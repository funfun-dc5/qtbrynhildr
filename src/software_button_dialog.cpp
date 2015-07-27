// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_button_dialog.cpp

// Common Header
#include "common.h"

// System Header
#include <iostream>

// Qt header
#include <QDialog>
#include <QHBoxLayout>

// Local Header
#include "software_button_dialog.h"

namespace qtbrynhildr {

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
// constructor
SoftwareButtonDialog::SoftwareButtonDialog(SoftwareButton *softwareButton,
										   QWidget *parent)
  :
  //  QDialog(parent),
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  softwareButton(softwareButton)
{
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(softwareButton);
  setLayout(layout);
  resize(softwareButton->size());
  setWindowTitle("Software Button - Test -");
}

} // end of namespace qtbrynhildr
