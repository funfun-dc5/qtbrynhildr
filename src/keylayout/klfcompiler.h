// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef KLFCOMPILER_H
#define KLFCOMPILER_H
// Common Header

// System Header

// Local Header
#include "key.h"

#define ID_SHIFTKEY_THROUGH	0
#define ID_SHIFTKEY_NEED	1
#define ID_SHIFTKEY_NONEED	2

#define ID_SECTION_GENERAL	0
#define ID_SECTION_KEYS		1
#define ID_SECTION_SOFTKEYS	2

#define ID_PLATFORM_WINDOWS	0
#define ID_PLATFORM_MACOS	1
#define ID_PLATFORM_LINUX	2

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int make_KLX(const char *infile);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KLFCOMPILER_H
