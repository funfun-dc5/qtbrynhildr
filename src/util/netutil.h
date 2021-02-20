// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

#ifndef NETUTIL_H
#define NETUTIL_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header


namespace qtbrynhildr {

class NetUtil
{
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  NetUtil();
  // destructor
  virtual ~NetUtil();

  // get mac address
  bool getMacAddress();
};

} // end of namespace qtbrynhildr

#endif // NETUTIL_H
