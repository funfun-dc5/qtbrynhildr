// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

#ifndef WAVE_H
#define WAVE_H
// Common Header

// Qt Header

// Local Header

namespace qtbrynhildr {

  // for WAVE format
  typedef unsigned int uint32_t;
  typedef signed int int32_t;
  typedef unsigned short int uint16_t;
  typedef signed short int int16_t;
  typedef unsigned char uint8_t;
  typedef signed char int8_t;

  // Riff
  struct RiffHeader {
	uint32_t	riff;
	int32_t		size;
	uint32_t	type;
  };

#define RIFF_ID 	"RIFF"
#define RIFF_TYPE	"WAVE"

  // FormatChunk
  struct FormatChunk {
	uint32_t	id;
	int32_t		size;
	int16_t		format;
	uint16_t	channels;
	uint32_t	samplerate;
	uint32_t	bytepersec;
	uint16_t	blockalign;
	uint16_t	bitswidth;
  };

#if !defined(WAVE_FORMAT_PCM)
#define WAVE_FORMAT_PCM		1
#endif

#define FORMAT_CHUNK_ID 	"fmt "
#define FORMAT_CHUNK_SIZE	(sizeof(FormatChunk)-8)

  // DataChunk
  struct DataChunk {
	uint32_t	id;
	uint32_t	size;
	//	uint8_t		waveformData[];
  };

#define DATA_CHUNK_ID		"data"

} // end of namespace qtbrynhildr

#endif // WAVE_H
