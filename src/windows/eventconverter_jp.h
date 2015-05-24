// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// eventconverter_jp.h

#ifndef EVENTCONVERTER_JP_H
#define EVENTCONVERTER_JP_H

// Qt Header

// Local Header
#include "eventconverter.h"
#include "keycodes.h"

namespace qtbrynhildr {

// EventConverter_JP
class EventConverter_JP : public EventConverter
{
protected:
  // get Virtual Keycode
  uchar getVKCode(Key key);

  // get name
  QString getEventConverterName();
};

} // end of namespace qtbrynhildr

#endif // EVENTCONVERTER_JP_H
