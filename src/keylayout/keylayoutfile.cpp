// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "keylayout/keylayoutfile.h"

namespace qtbrynhildr {

// constructor
KeyLayoutFile::KeyLayoutFile(const char *keyLayoutFileImage)
  :klfImage(keyLayoutFileImage)
{
  KLFHeader *header = (KLFHeader*)keyLayoutFileImage;

  // KLF Header
  spec = header->spec;
  keynum = header->keynum;
  softkeynum = header->softkeynum;

  // [General] section
  name = header->name;
  author = header->author;

  // [Keys] section
  keyEventTable = (EventConverter::KeyEvent*)(klfImage + sizeof(KLFHeader));

  // [SoftKeys] section
  keyTopTable = (SoftwareKeyboard::KeyTop*)(klfImage + sizeof(KLFHeader) +
											sizeof(EventConverter::KeyEvent) * keynum);
}

// destructor
KeyLayoutFile::~KeyLayoutFile()
{
  // delete objects
  if (klfImage != 0){
	delete [] klfImage;
	klfImage = 0;
  }
}

// get name of key layout
const char* KeyLayoutFile::getName()
{
  return (const char*)name;
}

} // end of namespace qtbrynhildr
