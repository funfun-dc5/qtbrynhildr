// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// cipher.h

#ifndef CIPHER_H
#define CIPHER_H
// Common Header
#include "common/common.h"

// System Header
#include <string>

// CryptoPP Header
#include "cryptlib.h"
#include "des.h"
#include "filters.h"
#include "hex.h"
#include "modes.h"

using namespace std;
using namespace CryptoPP;

namespace qtbrynhildr {

// Cipher
class Cipher {
public:
  // contructor
  Cipher(const char *key);

  // encode string
  string encodeString(string plain);

  // decode string
  string decodeString(string encoded);

private:
  ECB_Mode<DES>::Encryption encctx;
  ECB_Mode<DES>::Decryption decctx;

  // output log flag
  bool outputLog;
};

} // end of namespace qtbrynhildr

#endif // CIPHER_H
