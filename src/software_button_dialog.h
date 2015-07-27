// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_button_dialog.h

#ifndef SOFTWARE_BUTTON_DIALOG_H
#define SOFTWARE_BUTTON_DIALOG_H
// Qt Header
#include <QDialog>

// Local Header
#include "software_button.h"

namespace qtbrynhildr {

// SoftwareButtonDialog
class SoftwareButtonDialog : public QDialog
{
  Q_OBJECT

public:
  // constructor
  SoftwareButtonDialog(SoftwareButton *softwareButton, QWidget *parent = 0);

private:
  // software keyboard
  SoftwareButton *softwareButton;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_BUTTON_DIALOG_H
