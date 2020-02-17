// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2017 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if !defined(__aarch64__)
#if !defined(__ARM_NEON__)
#if _MSC_VER
#include <intrin.h>
#else // _MSC_VER
#include <cpuid.h>
#endif // _MSC_VER
#endif // !defined(__ARM_NEON__)
#endif // !defined(__aarch64__)

// Qt Header

// Local Header
#include "cpuinfo.h"


namespace qtbrynhildr {

const CPUInfo::CPUInfo_Internal CPUInfo::CPUInformation;

#if !defined(__aarch64__)

#if !defined(__ARM_NEON__)

#if _MSC_VER

// get cpuid
void CPUInfo::getCPUID(int function_id, int data[4])
{
  __cpuid(data, function_id);
}
// get cpuidex
void CPUInfo::getCPUIDEX(int function_id, int subfunction_id, int data[4])
{
  __cpuidex(data, function_id, subfunction_id);
}

#elif defined(__GNUC__)

// get cpuid
void CPUInfo::getCPUID(int function_id, int data[4])
{
  __asm__ volatile ("cpuid"
					:"=a"(data[0]), "=b"(data[1]), "=c"(data[2]), "=d"(data[3])
					: "a" (function_id), "c" (0));
}
// get cpuidex
void CPUInfo::getCPUIDEX(int function_id, int subfunction_id, int data[4])
{
  __cpuid_count(function_id, subfunction_id, data[0], data[1], data[2], data[3]);
}

#endif // defined(__GNUC__)

#endif // !defined(__ARM_NEON__)

#endif // !defined(__aarch64__)

} // end of namespace qtbrynhildr
