// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef VERSION_H
#define VERSION_H

// version
#define QTB_VERSION				"2.4.3"

#define QTB_RCNAME				"-rc1"
#define QTB_VERSION_NUMBER		2403

#if __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (64bit)"
#else // __x86_64__ || __LP64__ || _WIN64
#define QTB_ARCHNAME			" (32bit)"
#endif // __x86_64__ || __LP64__ || _WIN64

// year
#define QTB_YEAR		"2015-2022"

#endif // VERSION_H
