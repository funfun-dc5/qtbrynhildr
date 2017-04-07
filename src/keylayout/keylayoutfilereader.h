// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILEREADER_H
#define KEYLAYOUTFILEREADER_H
// Common Header
#include "common/common.h"

// System Header

// Qt Header
#include <QList>

// Local Header
#include "keylayout/keylayoutfile.h"
#include "touchpanel2/software_keyboard.h"
#include "windows/eventconverter.h"

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
  // key layout file path
  const char *path;

  // key layout list
  typedef struct {
	QString name;	// Name
	QString author;	// Author
	int spec;		// SpecVersion
	int keynum;		// num of KeyEvent
	int softkeynum;	// num of KeyTop
	char *klfImage;	// KFL image pointer
	EventConverter::KeyEvent *keyEvent;	// KeyEvent pointer
	SoftwareKeyboard::KeyTop *keyTop;	// KeyTop pointer
  } KeyLayout;
  QList<KeyLayout> list;
};

} // end of namespace qtbrynhildr

#endif // KEYLAYOUTFILEREADER_H
