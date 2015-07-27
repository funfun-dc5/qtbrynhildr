// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// sofware_button.h

#ifndef SOFTWARE_BUTTON_H
#define SOFTWARE_BUTTON_H
// Qt Header
#include <QWidget>

// Local Header
#include "mousebuffer.h"
#include "ui_software_button.h"

namespace qtbrynhildr {

// SoftwareButton
class SoftwareButton : public QWidget, public Ui::SoftwareButton
{
  Q_OBJECT

public:
  // constructor
  SoftwareButton(MouseBuffer *mouseBuffer, QWidget *parent = 0);

private:
  // mouse buffer
  MouseBuffer *mouseBuffer;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // SOFTWARE_BUTTON_H
