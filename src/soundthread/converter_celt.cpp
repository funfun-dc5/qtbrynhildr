// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015- FunFun <fu.aba.dc5@gmail.com>

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
  ,frameSize(0)
  ,mode(0)
  ,decoder(0)
  ,workBuffer(0)
{
  int error;

  //  std::cout << "samplerate : " << samplerate << std::endl << std::flush;
  //  std::cout << "channels : " << channels << std::endl << std::flush;

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
  //  std::cout << "frameSize : " << frameSize << std::endl << std::flush;
  for(int decodedDataSize = 0 ; decodedDataSize < size; ){
	// decode 1 CELT data chunk
#if 1 // for miss align
	celt_int32 chunkSize = *chunkTop++;
	chunkSize |= *(chunkTop++) << 8;
	chunkSize |= *(chunkTop++) << 16;
	chunkSize |= *(chunkTop++) << 24;
	//	std::cout << "chunkSize : " << chunkSize << std::endl << std::flush;
#else // 0 // for miss align
	celt_int32 chunkSize = *((celt_int32 *)(chunkTop));
	//	std::cout << "chunkSize : " << chunkSize << std::endl << std::flush;
	chunkTop += 4;
#endif // 0 // for miss align
	error = celt_decode(decoder, chunkTop, chunkSize, workTop, frameSize);
	if (error != 0){
	  // decode error
	  std::cout << "celt_decode() error! : " << error << std::endl << std::flush;
	}

	// for next chunk
	decodedDataSize += (chunkSize + 4);
	chunkTop += chunkSize;
	workTop += pcmSize;
	//	std::cout << "workTop : " << workTop << std::endl << std::flush;
  }
  decodedPCMSize = (int)((char*)workTop - buffer);
  if (decodedPCMSize == 0){
	return 0;
  }

  // 2) return decoded PCM data size
  //  std::cout << "CELT       : " << size << std::endl << std::flush;
  //  std::cout << "convert PCM: " << decodedPCMSize << std::endl << std::flush;
  return decodedPCMSize;
}

} // end of namespace qtbrynhildr
