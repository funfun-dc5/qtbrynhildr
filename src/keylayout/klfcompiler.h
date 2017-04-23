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
#define ID_PLATFORM_LINUX	1
#define ID_PLATFORM_MACOS	2
#define ID_PLATFORM_NUM		3

#if defined(PLATFORM_WINDOWS)
#define THIS_PLATFORM ID_PLATFORM_WINDOWS
#elif defined(PLATFORM_LINUX)
#define THIS_PLATFORM ID_PLATFORM_LINUX
#elif defined(PLATFORM_MACOS)
#define THIS_PLATFORM ID_PLATFORM_MACOS
#else
#error "NOT support platform!"
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int lineno;

int make_KLX(const char *infile, const char *outfile);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KLFCOMPILER_H
