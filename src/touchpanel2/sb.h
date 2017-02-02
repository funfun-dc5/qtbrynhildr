// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef SB_H
#define SB_H

// System Header

// Qt Header

// Local Header
#include "control/mousebuffer.h"
#include "settings.h"
#include "software_button.h"

namespace qtbrynhildr {

// SoftwareButton
class SB : public SoftwareButton
{
  Q_OBJECT

public:
  // constructor
  SB(Settings *settings, MouseBuffer *mouseBuffer, QWidget *parent = 0);
  // destructor
  //  ~SB();

protected:
  // pressed button
  void pressedButton(ID_BUTTON id) override;

  // released button
  void releasedButton(ID_BUTTON id) override;

private:
  // settings
  Settings *settings;

  // mouse buffer
  MouseBuffer *mouseBuffer;

  // previous click button
  MouseInfoType previousClick;

  // previous click time
  QDateTime previousClickTime;

  // output log flag
  bool outputLog;

signals:
  // refresh menu
  void refreshMenu();
};

} // end of namespace qtbrynhildr

#endif // SB_H
