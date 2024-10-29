// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018- FunFun <fu.aba.dc5@gmail.com>
#if defined(__x86_64__) // for Universal Binary

// Common Header
#include "common/common.h"

// System Header
#if _MSC_VER
#include <intrin.h>
#else // !_MSC_VER
#include <x86intrin.h>
#endif // !_MSC_VER

// Qt Header

// Local Header
#include "yuv2rgb.h"

namespace qtbrynhildr {

#if QTB_SIMD_SUPPORT

// SSE

#if defined(__SSE4_2__)

// YUV convert to RGB (SIMD version)
void convertYUVtoRGB_SIMD_SSE(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  Aligned(16) int result[4];

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

#if 1 // for improvement

  __m128i yc = _mm_setr_epi32(256, 256, 256, 0);
  __m128i uc = _mm_setr_epi32(0,   -88, 453, 0);
  __m128i vc = _mm_setr_epi32(358,-182,   0, 0);

  __m128i constMaxV = _mm_setr_epi32(255, 255, 255, 255);
  //  __m128i constMinV = _mm_setr_epi32(0, 0, 0, 0);
  __m128i constMinV = _mm_setzero_si128();

#else // 0 // for improvement

  Aligned(16) const int constYc[4] = {256, 256, 256, 0};
  Aligned(16) const int constUc[4] = {0,   -88, 453, 0};
  Aligned(16) const int constVc[4] = {358,-182,   0, 0};

  // 2) load Yc, Uc, Vc
  __m128i yc = _mm_load_si128((const __m128i*)constYc);
  __m128i uc = _mm_load_si128((const __m128i*)constUc);
  __m128i vc = _mm_load_si128((const __m128i*)constVc);

  Aligned(16) const int constMax[4] = {255, 255, 255, 255};
  Aligned(16) const int constMin[4] = {  0,   0,   0,   0};

  __m128i constMaxV = _mm_load_si128((const __m128i*)constMax);
  __m128i constMinV = _mm_load_si128((const __m128i*)constMin);

#endif // 0 // for improvement

  Aligned(16) int ya[4] = {  0,   0,   0,   0};
  Aligned(16) int ua[4] = {  0,   0,   0,   0};
  Aligned(16) int va[4] = {  0,   0,   0,   0};

#if QTB_BENCHMARK
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
  // calc rate
  calcRate = 0.0;
#endif // QTB_BENCHMARK

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  __m128i yv, uv, vv;
	  __m128i uv0, vv0;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;
	  ua[0] = u;
	  ua[1] = u;
	  ua[2] = u;
	  va[0] = v;
	  va[1] = v;
	  va[2] = v;

	  // load U, V
	  uv0 = _mm_load_si128((const __m128i*)ua);
	  vv0 = _mm_load_si128((const __m128i*)va);

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// xPos

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  ya[0] = y;
		  ya[1] = y;
		  ya[2] = y;

		  // 1) load Y
		  yv = _mm_load_si128((const __m128i*)ya);

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)result[2];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}

		// xPos+1

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  ya[0] = y;
		  ya[1] = y;
		  ya[2] = y;

		  // 1) load Y
		  yv = _mm_load_si128((const __m128i*)ya);

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)result[2];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// xPos

		// set y
		y =  *ytop++;
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)result[2];
		// G
		*rgbtop++ = (uchar)result[1];
		// R
		*rgbtop++ = (uchar)result[0];
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		// xPos+1

		// set y
		y =  *ytop++;
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)result[2];
		// G
		*rgbtop++ = (uchar)result[1];
		// R
		*rgbtop++ = (uchar)result[0];
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#endif // !QTB_LITTLE_ENDIAN
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

// clip
inline int clip(int val)
{
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}

// RGB convert to Gray Scale macro
#define GET_GS(R, G, B) (R * 0.298912 + G * 0.586611 + B * 0.114578)

// YUV convert to RGB (SIMD version) (Gray Scale Version)
void convertYUVtoRGB_SIMD_SSE_GSV(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  Aligned(16) int result[4];

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

#if 1 // for improvement

  __m128i yc = _mm_setr_epi32(256, 256, 256, 0);
  __m128i uc = _mm_setr_epi32(0,   -88, 453, 0);
  __m128i vc = _mm_setr_epi32(358,-182,   0, 0);

  __m128i constMaxV = _mm_setr_epi32(255, 255, 255, 255);
  //  __m128i constMinV = _mm_setr_epi32(0, 0, 0, 0);
  __m128i constMinV = _mm_setzero_si128();

#else // 0 // for improvement

  Aligned(16) const int constYc[4] = {256, 256, 256, 0};
  Aligned(16) const int constUc[4] = {0,   -88, 453, 0};
  Aligned(16) const int constVc[4] = {358,-182,   0, 0};

  // 2) load Yc, Uc, Vc
  __m128i yc = _mm_load_si128((const __m128i*)constYc);
  __m128i uc = _mm_load_si128((const __m128i*)constUc);
  __m128i vc = _mm_load_si128((const __m128i*)constVc);

  Aligned(16) const int constMax[4] = {255, 255, 255, 255};
  Aligned(16) const int constMin[4] = {  0,   0,   0,   0};

  __m128i constMaxV = _mm_load_si128((const __m128i*)constMax);
  __m128i constMinV = _mm_load_si128((const __m128i*)constMin);

#endif // 0 // for improvement

  Aligned(16) int ya[4] = {  0,   0,   0,   0};
  Aligned(16) int ua[4] = {  0,   0,   0,   0};
  Aligned(16) int va[4] = {  0,   0,   0,   0};

#if QTB_BENCHMARK
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
  // calc rate
  calcRate = 0.0;
#endif // QTB_BENCHMARK

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  __m128i yv, uv, vv;
	  __m128i uv0, vv0;
	  int r, g, b, gsv;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;
	  ua[0] = u;
	  ua[1] = u;
	  ua[2] = u;
	  va[0] = v;
	  va[1] = v;
	  va[2] = v;

	  // load U, V
	  uv0 = _mm_load_si128((const __m128i*)ua);
	  vv0 = _mm_load_si128((const __m128i*)va);

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// xPos

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  ya[0] = y;
		  ya[1] = y;
		  ya[2] = y;

		  // 1) load Y
		  yv = _mm_load_si128((const __m128i*)ya);

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

		  // R
		  r = result[0];
		  // G
		  g = result[1];
		  // B
		  b = result[2];
		  // gray scale
		  gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // B
		  *rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // B
		  *rgbtop++ = (uchar)gsv;
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}

		// xPos+1

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  ya[0] = y;
		  ya[1] = y;
		  ya[2] = y;

		  // 1) load Y
		  yv = _mm_load_si128((const __m128i*)ya);

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

		  // R
		  r = result[0];
		  // G
		  g = result[1];
		  // B
		  b = result[2];
		  // gray scale
		  gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // B
		  *rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)gsv;
		  // G
		  *rgbtop++ = (uchar)gsv;
		  // B
		  *rgbtop++ = (uchar)gsv;
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// xPos

		// set y
		y =  *ytop++;
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		r = result[0];
		// G
		g = result[1];
		// B
		b = result[2];
		// gray scale
		gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// B
		*rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// R
		*rgbtop++ = (uchar)gsv;
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// B
		*rgbtop++ = (uchar)gsv;
#endif // !QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32

		// xPos+1

		// set y
		y =  *ytop++;
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

		// R
		r = result[0];
		// G
		g = result[1];
		// B
		b = result[2];
		// gray scale
		gsv = clip(GET_GS(r, g, b));

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// B
		*rgbtop++ = (uchar)gsv;
#elif FORMAT_RGB32
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// R
		*rgbtop++ = (uchar)gsv;
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // !QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)gsv;
		// G
		*rgbtop++ = (uchar)gsv;
		// B
		*rgbtop++ = (uchar)gsv;
#endif // !QTB_LITTLE_ENDIAN
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

#endif // defined(__SSE4_2__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr

#endif // defined(__x86_64__) // for Universal Binary
