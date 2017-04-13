// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef KEYTOP_H
#define KEYTOP_H
// Common Header

// System Header

// Qt Header

// Local Header

#ifdef __cplusplus
public:
#endif // __cplusplus

#ifdef __cplusplus
private:
#else // __cplusplus
typedef unsigned char uchar;
#endif // __cplusplus

  // key top information
  typedef struct {
	struct {
	  char keyTop[10];
	  char keyTopWithShift[10];
	  uchar VK_Code;
	} keyTop;
	struct {
	  char keyTop[10];
	  uchar VK_Code;
	} keyTopWithFn;
  } KeyTop;
#endif // KEYTOP_H
