// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILEREADER_H
#define KEYLAYOUTFILEREADER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QList>

// Local Header
#include "keylayout/keylayoutfile.h"

namespace qtbrynhildr {

// Key Layout File Reader
class KeyLayoutFileReader {
  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // key layout file path
  const char *path;

  // key layout file list
  QList<KeyLayoutFile*> list;

  // Keyboard type list
  QStringList keyboardTypeList;

  // output log flag
  const bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  KeyLayoutFileReader(const char *layoutfilepath);
  // destructor
  ~KeyLayoutFileReader();

  // get key layout file
  KeyLayoutFile* getKeyLayoutFile(int index);

  // get keyboard type list
  QStringList getKeyboardTypeList();

  // get index of keyboard type
  int getIndexOfKeyboardType(QString keyboardTypeName);

private:
  // read a key layout file
  void readKeyLayoutFile(const char *file);
};

} // end of namespace qtbrynhildr

#endif // KEYLAYOUTFILEREADER_H
