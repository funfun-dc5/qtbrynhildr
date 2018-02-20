// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

#ifndef CPUINFO_H
#define CPUINFO_H
// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header

// Local Header


#if !defined(__ARM_NEON__)

namespace qtbrynhildr {

class CPUInfo
{
  class CPUInfo_Internal;

  //-------------------------------------------------------------------------------
  // Variable
  //-------------------------------------------------------------------------------
private:
  static const CPUInfo_Internal CPUInformation;

  class CPUInfo_Internal
  {
  public:
	bool isIntel;
	bool isAMD;
	bool hasSSE41;
	bool hasSSE42;
	bool hasAVX;
	bool hasAVX2;
	bool hasNEON;

	CPUInfo_Internal()
	  :isIntel(false)
	  ,isAMD(false)
	  ,hasSSE41(false)
	  ,hasSSE42(false)
	  ,hasAVX(false)
	  ,hasNEON(false)
	{
#if !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
	  int data[4];

	  // get vender
	  char vender[0x20];
	  memset(vender, 0, sizeof(vender));
	  getCPUID(0, data);
	  memcpy(&vender[0], &data[1], 4);
	  memcpy(&vender[4], &data[3], 4);
	  memcpy(&vender[8], &data[2], 4);
	  cout << "vender = " << vender << endl << flush;
	  if (strncmp(vender, "GenuinIntel", 0x20) == 0){
		isIntel = true;
	  }
	  else if (strncmp(vender, "AuthenticAMD", 0x20) == 0){
		isAMD = true;
	  }

	  // SSE41
	  getCPUID(1, data);
	  hasSSE41 = ((data[2] >> 19) & 1) != 0;
	  hasSSE42 = ((data[2] >> 20) & 1) != 0;
	  hasAVX   = ((data[2] >> 28) & 1) != 0;
#if 1 // for TEST
	  if (hasSSE41){
		cout << "SSE4.1 : Supported" << endl;
	  }
	  else {
		cout << "SSE4.1 : NOT Supported" << endl;
	  }
	  if (hasSSE42){
		cout << "SSE4.2 : Supported" << endl;
	  }
	  else {
		cout << "SSE4.2 : NOT Supported" << endl;
	  }
	  if (hasAVX){
		cout << "AVX    : Supported" << endl;
	  }
	  else {
		cout << "AVX    : NOT Supported" << endl;
	  }
	  cout << flush;
#endif // 0 // for TEST
#endif // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))

	  // for TEST
	  hasNEON = true;
	}
  };

  // output log flag
  bool outputLog;

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
#if !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
  // for Intel CPU
  static bool SSE41(void) { return CPUInformation.hasSSE41; } // for TEST
#else // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
  // for ARM CPU
  static bool NEON(void) { return CPUInformation.hasNEON; } // for TEST
#endif // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))

private:
  // constructor
  CPUInfo();
  // destructor
  ~CPUInfo();

  // for Intel CPU
#if !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
  // get cpuid
  static void getCPUID(int function_id, int data[4]);

  // get cpuidex
  static void getCPUIDEX(int function_id, int subfunction_id, int data[4]);
#endif // !(defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
};

} // end of namespace qtbrynhildr

#endif // !defined(__ARM_NEON__)

#endif // CPUINFO_H
