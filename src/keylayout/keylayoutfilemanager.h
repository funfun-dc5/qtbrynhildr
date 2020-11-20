// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILEMANAGER_H
#define KEYLAYOUTFILEMANAGER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header

namespace qtbrynhildr {

// Key Layout File Manager
class KeyLayoutFileManager {

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // key layout file path
  //const char *path;

  // output log flag
  const bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  KeyLayoutFileManager(const char *layoutfilepath);
  // destructor
  ~KeyLayoutFileManager();
};

} // end of namespace qtbrynhildr

#endif // KEYLAYOUTFILEMANAGER_H
