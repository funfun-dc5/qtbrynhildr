// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYLAYOUTFILE_H
#define KEYLAYOUTFILE_H
// Common Header

// System Header

// Local Header
#include "key.h"
#include "windows/keycodes.h"

// Key Layout File Header
#if _MSC_VER
#pragma pack(push, 1)
typedef struct { // for C/C++ Compatibility
#else // _MSC_VER
typedef struct __attribute__((__packed__)) { // for C/C++ Compatibility
#endif // _MSC_VER
  // File Header
  char	magic[4];	// file magic
  char	padding[4];	// padding
  int	spec;		// SpecVersion
  int	size;		// file size
  int	keynum;		// Keys Entry Num
  int	softkeynum;	// SoftKeys Entry Num

  // [General] Section
  char	name[64];	// Name strings
  char	author[64];	// Author strings
} KLFHeader;
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

// -- File Structure --
//
// 1) KLFHeader
// 2) Keys Entries     (x keynum)
// 3) SoftKeys Entrise (x softkeynum)

#endif // KEYLAYOUTFILE_H
