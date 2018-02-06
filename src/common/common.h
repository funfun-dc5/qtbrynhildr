// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef COMMON_H
#define COMMON_H
// Qt Header
#include <Qt>

// Local Header
#include "config.h"
#include "util.h"

// Utilities
// for C++11
#if !_MSC_VER && __cplusplus < 201103L
define constexpr /* */
#endif // !_MSC_VER && __cplusplus < 201103L

// for align
#if defined(_MSC_VER)
#define Aligned(n)  __declspec(align(n))
#else // defined(_MSC_VER)
#define Aligned(n)  __attribute__((aligned(n)))
#endif // defined(_MSC_VER)

#endif // COMMON_H
