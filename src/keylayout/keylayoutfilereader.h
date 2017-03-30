// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILEREADER_H
#define KEYLAYOUTFILEREADER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "keylayout/keylayoutfile.h"

namespace qtbrynhildr {

// Key Layout File Reader
class KeyLayoutFileReader {
public:
  // constructor
  KeyLayoutFileReader(const char *layoutfilepath);
  // destructor
  ~KeyLayoutFileReader();

private:
  // read a key layout file
  void readKeyLayoutFile(const char *file);

private:
  // layout file path
  const char *path;
};

} // end of namespace qtbrynhildr

#endif // KEYLAYOUTFILEREADER_H
