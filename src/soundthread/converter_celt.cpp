// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header

// Local Header
#include "converter_celt.h"

namespace qtbrynhildr {

// constructor
Converter_CELT::Converter_CELT(int samplerate, int channels)
  :Converter(samplerate, channels)
  ,mode(0)
{
  int error;

  //  cout << "samplerate : " << samplerate << endl << flush;
  //  cout << "channels : " << channels << endl << flush;

  // 0) frame size
  frameSize = (samplerate > 40000) ? 48 : 40;

  // 1) create CELT mode
  mode = celt_mode_create(samplerate, frameSize, &error);

  // 2) create CELT decoder
  decoder = celt_decoder_create_custom(mode, channels, &error);

  // 3) work buffer
  workBuffer = new char[512*1024];
}

// destructor
Converter_CELT::~Converter_CELT()
{
  // 1) delete CELT decoder
  if (decoder != 0){
	celt_decoder_destroy(decoder);
	decoder = 0;
  }

  // 2) delete CELT mode
  if (mode != 0){
	celt_mode_destroy(mode);
	mode = 0;
  }

  // 3) work buffer
  if (workBuffer != 0){
	delete [] workBuffer;
	workBuffer = 0;
  }
}

// convert to PCM
int Converter_CELT::convertToPCM(char *buffer, int size)
{
  int decodedPCMSize = 0;

  // decode CELT Stream
  // format:
  // size (4 bytes/Little Endian) + CELT data + size (4 bytes/Little Endian) + CELT data + ...

  // 1) copy raw data to workBuffer
  memcpy(workBuffer, buffer, size);

  // 2) decode all CELT data chunks to PCM data (buffer)
  unsigned char *chunkTop = (unsigned char*)workBuffer;
  celt_int16 *workTop = (celt_int16*)buffer;
  int pcmSize = channels * frameSize;
  int error;
  //  cout << "frameSize : " << frameSize << endl << flush;
  for(int decodedDataSize = 0 ; decodedDataSize < size; ){
	// decode 1 CELT data chunk
#if 1 // for TEST
	// for miss align
	celt_int32 chunkSize = *chunkTop++;
	chunkSize |= *(chunkTop++) << 8;
	chunkSize |= *(chunkTop++) << 16;
	chunkSize |= *(chunkTop++) << 24;
	//	cout << "chunkSize : " << chunkSize << endl << flush;
#else // 1 // for TEST
	celt_int32 chunkSize = *((celt_int32 *)(chunkTop));
	//	cout << "chunkSize : " << chunkSize << endl << flush;
	chunkTop += 4;
#endif // 1 // for TEST
	error = celt_decode(decoder, chunkTop, chunkSize, workTop, frameSize);
	if (error != 0){
	  // decode error
	  cout << "celt_decode() error! : " << error << endl << flush;
	}

	// for next chunk
	decodedDataSize += (chunkSize + 4);
	chunkTop += chunkSize;
	workTop += pcmSize;
	//	cout << "workTop : " << workTop << endl << flush;
  }
  decodedPCMSize = (char*)workTop - buffer;
  if (decodedPCMSize == 0){
	return 0;
  }

  // 2) return decoded PCM data size
  //  cout << "CELT       : " << size << endl << flush;
  //  cout << "convert PCM: " << decodedPCMSize << endl << flush;
  return decodedPCMSize;
}

} // end of namespace qtbrynhildr
