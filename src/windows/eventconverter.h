// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// eventconverter.h

#ifndef EVENTCONVERTER_H
#define EVENTCONVERTER_H

// Qt Header
#include <QString>
#include <Qt>

// Local Header
#include "keycodes.h"

using namespace Qt;

namespace qtbrynhildr {

// EventConverter
class EventConverter
{
public:
  // constructor
  EventConverter();

  // get Virtual Keycode
  virtual uchar getVKCode(Key key) = 0;

  // get name
  virtual QString getEventConverterName() = 0;

  // for DEBUG
  static QString getVKCodeByString(uchar vkcode);

protected:
  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // EVENTCONVERTER_H
