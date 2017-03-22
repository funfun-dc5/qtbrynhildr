// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef LANGFILE_H
#define LANGFILE_H

// System Header

// Local Header

typedef struct {
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
} LFHeader;

// -- File Structure --
//
// 1) LFHeader
// 2) Keys Entries     (x keynum)
// 3) SoftKeys Entrise (x softkeynum)

#endif // LANGFILE_H
