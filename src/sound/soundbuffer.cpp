// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2015 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Local Header
#include "soundbuffer.h"

namespace qtbrynhildr {

// constructor
SoundBuffer::SoundBuffer(int size)
  :
  topPos(0),
  nextPos(0),
  // for DEBUG
  outputLog(false)
{
  // allocate buffer
  buffer = new char[size];
  pairBuffer = new char[size];
  bufferSize = size;
}

// destructor
SoundBuffer::~SoundBuffer()
{
  if (buffer != 0){
	delete [] buffer;
	buffer = 0;
  }
  if (pairBuffer != 0){
	delete [] pairBuffer;
	pairBuffer = 0;
  }
}

// put data to ring buffer
int SoundBuffer::put(const char *buf, int len)
{
  // check parameters
  if (buf == 0){
	return -1;
  }
  if (len <= 0){
	return -1;
  }
  if (len > bufferSize){
	// too big to copy buf to buffer
	return -1;
  }

  // check buffer overflow
  if (topPos < nextPos){
	// ----------------------------
	// |   |----------------|     |
	// ----------------------------
	//    topPos            nextPos
	if (topPos + bufferSize - nextPos < len){
	  return -1;
	}
  }
  else if (topPos > nextPos){
	// ----------------------------
	// |----|               |-----|
	// ----------------------------
	//       nextPos         topPos
	if (topPos - nextPos < len){
	  return -1;
	}
  }

  // copy buf to buffer
  if (nextPos + len < bufferSize){
	// copy buf to buffer by 1 step
	memcpy(buffer + nextPos, buf, len);

	nextPos += len;
  }
  else if (nextPos + len > bufferSize){
	int firstLen = bufferSize - nextPos ;
	// copy buf to buffer by 2 step
	// first copy step
	memcpy(buffer + nextPos, buf, firstLen);

	// second copy step
	memcpy(buffer, buf + firstLen, len - firstLen);

	nextPos = len - firstLen;
  }
  else { // nextPos + len == bufferSize
	// copy buf to buffer by 1 step
	memcpy(buffer + nextPos, buf, len);

	nextPos = 0;
  }

  return len;
}

// get data from ring buffer
const char *SoundBuffer::get(int len)
{
  char *retPtr = 0;

  if (len < 0){
	return retPtr;
  }
  if (topPos == nextPos){
	return retPtr;
  }

  if (topPos < nextPos){
	// ----------------------------
	// |   |----------------|     |
	// ----------------------------
	//    topPos            nextPos

	int bufferLen = nextPos - topPos;

	// return pointer to buffer
	retPtr = buffer + topPos;

	if (bufferLen > len){
	  topPos += len;
	}
	else {
	  topPos += bufferLen;
	}
  }
  else if (topPos > nextPos){
	// ----------------------------
	// |----|               |-----|
	// ----------------------------
	//       nextPos         topPos

	if (bufferSize - topPos > len){
	  // return pointer to buffer
	  retPtr = buffer + topPos;

	  topPos += len;
	}
	else if (bufferSize - topPos < len){
	  int firstLen = bufferSize - topPos;
	  // return pointer to pairBuffer
	  retPtr = pairBuffer;
	  // copy buffer to pairBuffer
	  memcpy(pairBuffer, buffer + topPos, firstLen);
	  memcpy(pairBuffer + firstLen, buffer, len - firstLen);
	  topPos = len - firstLen;
	}
	else { // bufferSize - topPos == len
	  // return pointer to buffer
	  retPtr = buffer + topPos;

	  topPos = 0;
	}
  }

  if (topPos == nextPos){
	topPos = 0;
	nextPos = 0;
  }

  return retPtr;
}

// clear buffer
void SoundBuffer::clear()
{
  // reset
  topPos = 0;
  nextPos = 0;
}

// get available data size
long SoundBuffer::size() const
{
  int len = 0;

  if (topPos < nextPos){
	// ----------------------------
	// |   |----------------|     |
	// ----------------------------
	//    topPos            nextPos
	len = nextPos - topPos;
  }
  else if (topPos > nextPos){
	// ----------------------------
	// |----|               |-----|
	// ----------------------------
	//       nextPos         topPos
	len = bufferSize - topPos + nextPos;
  }

  return len;
}

// get available data size (max size = len)
long SoundBuffer::size(int len) const
{
  int bufferLen = size();

  if (len <= bufferLen){
	return len;
  }
  else {
	return bufferLen;
  }
}

} // end of namespace qtbrynhildr
