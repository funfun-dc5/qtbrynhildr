// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef MSVC_H
#define MSVC_H

// C4996 warning
#define _CRT_SECURE_NO_WARNINGS

// for ISO C and C++
#define fileno _fileno
#define strdup _strdup
#include <io.h>
#define isatty _isatty

#endif // MSVC_H
