// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILE_H
#define KEYLAYOUTFILE_H
// Common Header

// System Header

// Local Header
#ifdef __cplusplus
#include "touchpanel2/software_keyboard.h"
#include "windows/eventconverter.h"
#endif // __cplusplus

// max length of string
#define MAX_STRING 63

// Key Layout File Header
#if _MSC_VER
#pragma pack(push, 1)
typedef struct { // for C/C++ Compatibility
#else // !_MSC_VER
typedef struct __attribute__((__packed__)) { // for C/C++ Compatibility
#endif // !_MSC_VER
  // File Header
  char	magic[4];	// file magic
  char	padding[4];	// padding
  int	spec;		// SpecVersion
  int	size;		// file size
  int	keynum;		// Keys Entry Num
  int	softkeynum;	// SoftKeys Entry Num

  // [General] Section
  char	name[MAX_STRING+1];	// Name strings
  char	author[MAX_STRING+1];	// Author strings
} KLFHeader;
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

// -- File Structure --
//
// 1) KLFHeader
// 2) Keys Entries     (x keynum)
// 3) SoftKeys Entrise (x softkeynum)

#ifdef __cplusplus
namespace qtbrynhildr {

class EventConverter;
class SoftwareKeyboard;

// Key Layout File
class KeyLayoutFile
{
  friend class EventConverter;
  friend class SoftwareKeyboard;

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  // pointer of KFL image
  const char *klfImage;

  // key layout name
  char *name;

  // key layout author
  char *author;

  // spec version
  int spec;

  // number of key event table entry
  int keynum;

  // number of key top table entry
  int softkeynum;

  // pointer of key event table entry
  EventConverter::KeyEvent *keyEventTable;

  // pointer of key top table entry
  SoftwareKeyboard::KeyTop *keyTopTable;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // constructor
  KeyLayoutFile(const char *layoutFileImage);
  // destructor
  ~KeyLayoutFile();

  // get name of key layout
  const char *getName();
};

} // end of namespace qtbrynhildr

#endif // __cplusplus

#endif // KEYLAYOUTFILE_H
