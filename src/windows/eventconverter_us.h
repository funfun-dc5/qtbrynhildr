// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// eventconverter_us.h

#ifndef EVENTCONVERTER_US_H
#define EVENTCONVERTER_US_H

// Qt Header

// Local Header
#include "eventconverter.h"
#include "keycodes.h"

namespace qtbrynhildr {

// EventConverter_US
class EventConverter_US : public EventConverter
{
protected:
  // get Virtual Keycode
  uchar getVKCode(Key key);

  // get name
  QString getEventConverterName();
};

} // end of namespace qtbrynhildr

#endif // EVENTCONVERTER_US_H
