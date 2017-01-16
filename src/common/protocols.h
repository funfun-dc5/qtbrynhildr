// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef PROTOCOLS_H
#define PROTOCOLS_H
// protocol version string
#define PROTOCOL_VERSION_STRING			"0000"
#define PROTOCOL_VERSION_STRING_LENGTH	4

// encryption key parameter
// key length
#define ENCRYPTION_KEY_LENGTH	16

#if __x86_64__ || __LP64__
#include "machine/protocols_x64.h"
#else // __x86_64__ || __LP64__
#include "machine/protocols_x86.h"
#endif // __x86_64__ || __LP64__

#endif // PROTOCOLS_H
