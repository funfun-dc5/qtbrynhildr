// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_keybord.h

#ifndef SOFTWARE_KEYBOARD_H
#define SOFTWARE_KEYBOARD_H
// Qt Header
#include <QWidget>

// Local Header
#include "ui_software_keyboard.h"

namespace qtbrynhildr {

// SoftwareKeyboard
class SoftwareKeyboard : public QWidget, public Ui::SoftwareKeyboard
{
  Q_OBJECT

public:
  // constructor
  SoftwareKeyboard(QWidget *parent = 0);

private:
  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_KEYBOARD_H
