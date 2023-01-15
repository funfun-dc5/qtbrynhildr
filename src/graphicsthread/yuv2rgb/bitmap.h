// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2020- FunFun <fu.aba.dc5@gmail.com>

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
typedef struct BITMAPFILEHEADER { // for C/C++ Compatibility
#else // !_MSC_VER
#if defined(__MINGW32__)
typedef struct __attribute__((gcc_struct, __packed__)) BITMAPFILEHEADER { // for C/C++ Compatibility
#else // !defined(__MINGW32__)
typedef struct __attribute__((__packed__)) BITMAPFILEHEADER { // for C/C++ Compatibility
#endif // !defined(__MINGW32__)
#endif // !_MSC_VER
	char		bfType[2]; // "BM"
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
  } BITMAPFILEHEADER;
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

  // information header
#if _MSC_VER
#pragma pack(push, 1)
typedef struct BITMAPINFOHEADER { // for C/C++ Compatibility
#else // !_MSC_VER
typedef struct __attribute__((__packed__)) BITMAPINFOHEADER { // for C/C++ Compatibility
#endif // !_MSC_VER
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
  } BITMAPINFOHEADER ;
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
typedef struct RGBQUAD { // for C/C++ Compatibility
#else // !_MSC_VER
typedef struct __attribute__((__packed__)) RGBQUAD { // for C/C++ Compatibility
#endif // !_MSC_VER
	uchar	rgbBlue;
	uchar	rgbGreen;
	uchar	rgbRed;
	uchar	rgbReserved; // 0
  } RGBQUAD;
#if _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

} // end of namespace qtbrynhildr

#endif // BITMAP_H
