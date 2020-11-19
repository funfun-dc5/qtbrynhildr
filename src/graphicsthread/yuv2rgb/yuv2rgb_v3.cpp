// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018-2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "yuv2rgb.h"

namespace qtbrynhildr {

// clip
inline int clip(int val)
{
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}

// YUV convert to RGB macro
#define GET_R(Y, V)		((Y           + 358 * V) >> 8)
#define GET_G(Y, U, V)	((Y -  88 * U - 182 * V) >> 8)
#define GET_B(Y, U)		((Y + 453 * U          ) >> 8)

// YUV convert to RGB (qtbrynhildr::convertYUVtoRGB())
void convertYUVtoRGB_CPP(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv == yuv1){
	yptop = y2topOrg + (ytop - y1topOrg);
	uptop = u2topOrg + (utop - u1topOrg);
	vptop = v2topOrg + (vtop - v1topOrg);
  }
  else {
	yptop = y1topOrg + (ytop - y2topOrg);
	uptop = u1topOrg + (utop - u2topOrg);
	vptop = v1topOrg + (vtop - v2topOrg);
  }

#if QTB_BENCHMARK
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
#endif // QTB_BENCHMARK

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// == xPos ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  y <<= 8; // y * 256

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}

		// == xPos+1 ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  y <<= 8; // y * 256

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  r = clip(GET_R(y, v));
		  *rgbtop++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgbtop++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgbtop++ = (uchar)b;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// == xPos ==
		y =  *ytop++;
		y <<= 8; // y * 256

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		// == xPos+1 ==
		y =  *ytop++;
		y <<= 8; // y * 256

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		yptop += 2;

#if QTB_BENCHMARK
		calcCounter += 2;
#endif // QTB_BENCHMARK
	  }
	}
#if !QTB_LOAD_BITMAP
	rgbtop += rgbNext;
#endif // !QTB_LOAD_BITMAP
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
#if QTB_BENCHMARK
  if (calcCounter + skipCounter > 0)
	calcRate = (double)calcCounter/(calcCounter + skipCounter) * 100.0;
#endif // QTB_BENCHMARK
}

#if QTB_GRAY_SCALE_MODE
// RGB convert to Gray Scale macro
#define GET_GS(R, G, B) (R * 0.299 + G * 0.587 + B * 0.114)

// YUV convert to RGB (qtbrynhildr::convertYUVtoRGB()) (Gray Scale Version)
void convertYUVtoRGB_CPP_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv == yuv1){
	yptop = y2topOrg + (ytop - y1topOrg);
	uptop = u2topOrg + (utop - u1topOrg);
	vptop = v2topOrg + (vtop - v1topOrg);
  }
  else {
	yptop = y1topOrg + (ytop - y2topOrg);
	uptop = u1topOrg + (utop - u2topOrg);
	vptop = v1topOrg + (vtop - v2topOrg);
  }

#if QTB_BENCHMARK
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
#endif // QTB_BENCHMARK

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;
	  int gsv;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// == xPos ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  y <<= 8; // y * 256

		  // R
		  r = clip(GET_R(y, v));
		  // G
		  g = clip(GET_G(y, u, v));
		  // B
		  b = clip(GET_B(y, u));
		  // gray scale
		  gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}

		// == xPos+1 ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  y <<= 8; // y * 256

		  // R
		  r = clip(GET_R(y, v));
		  // G
		  g = clip(GET_G(y, u, v));
		  // B
		  b = clip(GET_B(y, u));
		  // gray scale
		  gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
		  *rgbtop++ = (uchar)gsv;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// == xPos ==
		y =  *ytop++;
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		// G
		g = clip(GET_G(y, u, v));
		// B
		b = clip(GET_B(y, u));
		// gray scale
		gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		// == xPos+1 ==
		y =  *ytop++;
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		// G
		g = clip(GET_G(y, u, v));
		// B
		b = clip(GET_B(y, u));
		// gray scale
		gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
		*rgbtop++ = (uchar)gsv;
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		yptop += 2;

#if QTB_BENCHMARK
		calcCounter += 2;
#endif // QTB_BENCHMARK
	  }
	}
#if !QTB_LOAD_BITMAP
	rgbtop += rgbNext;
#endif // !QTB_LOAD_BITMAP
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
#if QTB_BENCHMARK
  if (calcCounter + skipCounter > 0)
	calcRate = (double)calcCounter/(calcCounter + skipCounter) * 100.0;
#endif // QTB_BENCHMARK
}
#endif // QTB_GRAY_SCALE_MODE

} // end of namespace qtbrynhildr
