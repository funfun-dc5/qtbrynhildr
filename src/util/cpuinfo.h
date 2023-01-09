// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017- FunFun <fu.aba.dc5@gmail.com>

#ifndef CPUINFO_H
#define CPUINFO_H
// Common Header
#include "common/common.h"

// System Header
#include <cstring>

// Qt Header

// Local Header


#if defined(__arm__) || defined(__aarch64__)
#if defined(QTB_ANDROID)
#include "util/android-ndk/cpu-features.h"
#endif // defined(QTB_ANDROID)
#endif // defined(__arm__) || defined(__aarch64__)

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
	// vender
	char vender[0x020];
	// brand
	char brand[0x40];

	// for INTEL CPU
	bool isIntel;
	bool isAMD;
	bool hasSSE41;
	bool hasSSE42;
	bool hasAVX;
	bool hasAVX2;
	bool hasFMA;

	// for ARM CPU
	bool hasNEON;

  private:
	// output log flag
	bool outputLog;

  public:
	CPUInfo_Internal()
	  :isIntel(false)
	  ,isAMD(false)
	  ,hasSSE41(false)
	  ,hasSSE42(false)
	  ,hasAVX(false)
	  ,hasAVX2(false)
	  ,hasNEON(false)
	  ,outputLog(false)
	{
	  memset(vender, 0, sizeof(vender));
	  memset(brand, 0, sizeof(brand));

#if !(defined(__arm__) || defined(__aarch64__))
	  // INTEL CPU
	  int data[4];

	  // get vender
	  //	  char vender[0x20];
	  //	  memset(vender, 0, sizeof(vender));
	  getCPUID(0, data);
	  memcpy(&vender[0], &data[1], 4);
	  memcpy(&vender[4], &data[3], 4);
	  memcpy(&vender[8], &data[2], 4);
	  if (strncmp(vender, "GenuinIntel", 0x20) == 0){
		isIntel = true;
	  }
	  else if (strncmp(vender, "AuthenticAMD", 0x20) == 0){
		isAMD = true;
	  }

	  // get brand
	  //	  char brand[0x40];
	  //	  memset(brand, 0, sizeof(brand));
	  for(int i = 0; i < 3; i++){
		getCPUID(0x80000002 + i, data);
		memcpy(&brand[16*i],  &data[0], 16);
	  }

	  // for SIMD
	  getCPUID(1, data);
	  hasFMA   = ((data[2] >> 12) & 1) != 0;
	  hasSSE41 = ((data[2] >> 19) & 1) != 0;
	  hasSSE42 = ((data[2] >> 20) & 1) != 0;
	  hasAVX   = ((data[2] >> 28) & 1) != 0;
#if 0 // for TEST
	  std::cout << "data[0] : 0x" << hex << data[0] << std::endl << std::flush;
	  std::cout << "data[1] : 0x" << hex << data[1] << std::endl << std::flush;
	  std::cout << "data[2] : 0x" << hex << data[2] << std::endl << std::flush;
	  std::cout << "data[3] : 0x" << hex << data[3] << std::endl << std::flush;
#endif // 0 // for TEST
	  getCPUID(7, data);
	  hasAVX2  = ((data[1] >>  5) & 1) != 0;
#if 0 // for TEST
	  std::cout << "data[0] : 0x" << hex << data[0] << std::endl << std::flush;
	  std::cout << "data[1] : 0x" << hex << data[1] << std::endl << std::flush;
	  std::cout << "data[2] : 0x" << hex << data[2] << std::endl << std::flush;
	  std::cout << "data[3] : 0x" << hex << data[3] << std::endl << std::flush;
#endif // 0 // for TEST

	  // for TEST
	  if (outputLog){
		std::cout << "vender = " << vender << std::endl << std::flush;

		std::cout << "brand  = " << brand << std::endl << std::flush;

		if (hasSSE41){
		  std::cout << "SSE4.1 : Supported" << std::endl;
		}
		else {
		  std::cout << "SSE4.1 : NOT Supported" << std::endl;
		}
		if (hasSSE42){
		  std::cout << "SSE4.2 : Supported" << std::endl;
		}
		else {
		  std::cout << "SSE4.2 : NOT Supported" << std::endl;
		}
		if (hasAVX){
		  std::cout << "AVX    : Supported" << std::endl;
		}
		else {
		  std::cout << "AVX    : NOT Supported" << std::endl;
		}
		if (hasAVX2){
		  std::cout << "AVX2   : Supported" << std::endl;
		}
		else {
		  std::cout << "AVX2   : NOT Supported" << std::endl;
		}
		if (hasFMA){
		  std::cout << "FMA    : Supported" << std::endl;
		}
		else {
		  std::cout << "FMA    : NOT Supported" << std::endl;
		}
		std::cout << std::flush;
	  }
#else // !(defined(__arm__) || defined(__aarch64__))
	  // ARM CPU
#if defined(QTB_ANDROID)
	  if ((android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM) &&
		  (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0){
		hasNEON = true;
	  }
#elif defined(QTB_IOS)
	  hasNEON = false;
#else // defined(QTB_ANDROID)
	  hasNEON = true;
#endif // defined(QTB_ANDROID)
	  // for TEST
	  if (outputLog){
		if (hasNEON){
		  std::cout << "NEON : Supported" << std::endl;
		}
		else {
		  std::cout << "NEON : NOT Supported" << std::endl;
		}
	  }
#endif // !(defined(__arm__) || defined(__aarch64__))
	}
  };

  //-------------------------------------------------------------------------------
  // Function
  //-------------------------------------------------------------------------------
public:
  // get vender
  static const char* getVener() { return CPUInformation.vender; }

  // get brand
  static const char* getBrand() { return CPUInformation.brand; }

#if !defined(__ARM_NEON__)
  // for Intel CPU
  static bool SSE42() { return CPUInformation.hasSSE42; }
  static bool AVX() { return CPUInformation.hasAVX; }
  static bool AVX2() { return CPUInformation.hasAVX2; }
#else // !defined(__ARM_NEON__)
  // for ARM CPU
  static bool NEON() { return CPUInformation.hasNEON; }
#endif // !defined(__ARM_NEON__)

private:
  // constructor
  CPUInfo();
  // destructor
  ~CPUInfo();

#if !defined(__ARM_NEON__)
  // for Intel CPU
  // get cpuid
  static void getCPUID(int function_id, int data[4]);

  // get cpuidex
  static void getCPUIDEX(int function_id, int subfunction_id, int data[4]);
#endif // !defined(__ARM_NEON__)
};

} // end of namespace qtbrynhildr

#endif // CPUINFO_H
