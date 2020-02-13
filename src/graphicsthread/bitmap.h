// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020 FunFun <fu.aba.dc5@gmail.com>

#ifndef BITMAP_H
#define BITMAP_H
// Common Header

// Qt Header

// Local Header

namespace qtbrynhildr {

  // for BITMAP format
  typedef unsigned int uint32_t;
  typedef signed int int32_t;
  typedef unsigned short int uint16_t;
  typedef signed short int int16_t;
  typedef unsigned char uint8_t;
  typedef signed char int8_t;

  // BITMAP for Windows

  // file header
#if _MSC_VER
#pragma pack(push, 1)
struct BITMAPFILEHEADER { // for C/C++ Compatibility
#else // _MSC_VER
#if defined(__MINGW32__)
struct __attribute__((gcc_struct, __packed__)) BITMAPFILEHEADER {
#else // defined(__MINGW32__)
struct __attribute__((__packed__)) BITMAPFILEHEADER {
#endif // defined(__MINGW32__)
#endif // _MSC_VER
	char		bfType[2]; // "BM"
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
  };
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

// constant value
#define BFTYPE_ID "BM"

  // information header
#if _MSC_VER
#pragma pack(push, 1)
struct BITMAPINFOHEADER {
#else // _MSC_VER
struct __attribute__((__packed__)) BITMAPINFOHEADER {
#endif // _MSC_VER
	uint32_t	biSize; // 40 for Windows
	uint32_t	biWidth;
	uint32_t	biHeight;
	uint16_t	biPlanes;
	uint16_t	biBitCount;
	uint32_t	biCompression;	// 0 (BI_RGB:NO compression)
	uint32_t	biSizeImage;
	int32_t		biXPixPerMeter;
	int32_t		biYPixPerMeter;
	uint32_t	biClrUsed;
	uint32_t	biCirImportant;
  };
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

// constant value
#if !defined(BI_RGB)
#define BI_RGB	0 // 0 (BI_RGB:NO compression)
#endif // !defined(BI_RGB)
#define BI_SIZE	40	// for Windows

  // pallet data
#if _MSC_VER
#pragma pack(push, 1)
struct RGBQUAD {
#else // _MSC_VER
  struct __attribute__((__packed__)) RGBQUAD {
#endif // _MSC_VER
	uchar	rgbBlue;
	uchar	rgbGreen;
	uchar	rgbRed;
	uchar	rgbReserved; // 0
  };
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

} // end of namespace qtbrynhildr

#endif // BITMAP_H
