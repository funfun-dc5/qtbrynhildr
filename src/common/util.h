// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

#ifndef UTIL_H
#define UTIL_H

// System Header
#include <cstdlib>
#include <iostream>

// Local Header

using namespace std;

// Assertion Macro
#if QTB_DEBUG
#define ASSERT(cond) \
  {																		\
	if (!(cond)) {														\
	  cout << "Assertion FAIL : "    << __FILE__ << " : LINE : " << dec << __LINE__ << endl; \
	  ::exit(EXIT_FAILURE);												\
	}																	\
  }
#else	// QTB_DEBUG
#define ASSERT(cond) /* no effect */
#endif	// QTB_DEBUG

// Abort Macro
#define ABORT() \
  { \
	cout << "Internal Error : "    << __FILE__ << " : LINE : " << dec << __LINE__ << endl; \
	::exit(EXIT_FAILURE);												\
  }

#endif // UTIL_H
