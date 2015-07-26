// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// software_keyboard_dialog.h

#ifndef SOFTWARE_KEYBOARD_DIALOG_H
#define SOFTWARE_KEYBOARD_DIALOG_H
// Qt Header
#include <QDialog>

// Local Header
#include "software_keyboard.h"

namespace qtbrynhildr {

// SoftwareKeyboardDialog
class SoftwareKeyboardDialog : public QDialog
{
  Q_OBJECT

public:
  // constructor
  SoftwareKeyboardDialog(SoftwareKeyboard *softwareKeyboard, QWidget *parent = 0);

private:
  // software keyboard
  SoftwareKeyboard *softwareKeyboard;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_DIALOG_H
