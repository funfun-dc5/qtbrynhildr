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

class QtBrynhildr;

// SoftwareButton
class SB : public SoftwareButton
{
  Q_OBJECT

public:
  // constructor
  SB(MouseBuffer *mouseBuffer, QtBrynhildr *qtbrynhildr, QWidget *parent);
  // destructor
  //  ~SB();

protected:
  // pressed button
  void pressedButton(ID_BUTTON id) override;

  // released button
  void releasedButton(ID_BUTTON id) override;

private:
  // mouse buffer
  MouseBuffer *mouseBuffer;

  // qtbrynhildr
  QtBrynhildr *qtbrynhildr;

  // settings
  Settings *settings;

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
