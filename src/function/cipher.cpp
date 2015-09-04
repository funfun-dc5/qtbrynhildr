// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>
// cipher.cpp

// Common Header
#include "common/common.h"

// System Header
#include <cstring>
#include <iostream>

// Local Header
#include "cipher.h"

using namespace std;

namespace qtbrynhildr {

// constructor
Cipher::Cipher(const char *key)
  :
  outputLog(false)
{
  byte cryptkey[DES::DEFAULT_KEYLENGTH];

  memcpy(cryptkey, key, DES::DEFAULT_KEYLENGTH );
  encctx.SetKey(cryptkey, DES::DEFAULT_KEYLENGTH );
  decctx.SetKey(cryptkey, DES::DEFAULT_KEYLENGTH );
}

string Cipher::encodeString(string plain)
{
  string cipher, encoded;

  // cipher
  StringSource(plain, true, 
			   new StreamTransformationFilter(encctx,
											  new StringSink( cipher )
											  )      
			   );

  // hex encode
  StringSource(cipher, true,
			   new HexEncoder(new StringSink(encoded)
							  )
			   );
  if (outputLog){
	cout << "plain  text: " << plain << endl;
	cout << "encode text: " << encoded << endl;
	cout << "decode text: " << decodeString(encoded) << endl;
  }

  return encoded;
}

string Cipher::decodeString(string encoded)
{
  string decoded, recovered;

  // hex decode
  StringSource(encoded, true,
			   new HexDecoder(new StringSink(decoded) 
							  )
			   );

  // cipher decode
  StringSource(decoded, true,
			   new StreamTransformationFilter(decctx,
											  new StringSink(recovered)
											  )
			   );

  return recovered;
}

} // end of namespace qtbrynhildr

#if 0 // for DEBUG
using namespace qtbrynhildr;

int main(int argc, char *argv[])
{
  Cipher cipher("Soul Gem");

  string encoded = cipher.encodeString("mcz-xoxo");
  cout << "encode text: " << encoded << endl;	
  string plain = cipher.decodeString(encoded);
  cout << "plain  text: " << plain << endl;	
}
#endif
